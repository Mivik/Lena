
#include <algorithm>
#include <unistd.h>
#include <vector>
#include <set>

#include <gtkmm.h>

#include "lena_client.h"
#include "common.h"
#include "log.h"

LenaClient::LenaClient() :
		broadcast_thread([this] { broadcast_thread_main(); }) {
	set_user_name(Glib::get_user_name());
	broadcast_thread.detach();
	file_socket = Gio::SocketService::create();
	file_socket->add_inet_port(LENA_CLIENT_FILE_PORT);
	file_socket->signal_incoming().connect(
			[this](const Glib::RefPtr<Gio::SocketConnection> &connection, const Glib::RefPtr<Glib::Object> &) {
				file_on_connected(connection);
				return false;
			});
	file_socket->start();
}

template<typename T>
inline void copy_string(const std::string &str, T &dst) {
	strncpy(dst, str.data(), sizeof(T));
}

std::string LenaClient::get_user_name() {
	broadcast_packet_mutex.lock();
	std::string ret = broadcast_packet.info.name;
	broadcast_packet_mutex.unlock();
	return ret;
}

void LenaClient::set_user_name(const std::string &name) {
	broadcast_packet_mutex.lock();
	copy_string(name, broadcast_packet.info.name);
	broadcast_packet_mutex.unlock();
}

Glib::RefPtr<Gio::File> LenaClient::get_work_dir() { return work_dir; }

void LenaClient::set_work_dir(const Glib::RefPtr<Gio::File> &file) {
	broadcast_packet_mutex.lock();
	work_dir = file;
	copy_string(file->get_path(), broadcast_packet.info.work_dir);
	broadcast_packet_mutex.unlock();
}

void LenaClient::init_packet() {
	Cena::ClientInfo &info = broadcast_packet.info;
	info.auto_launch = false;
	copy_string(Glib::get_host_name(), info.os);
	info.use_computer_name = false;
	broadcast_packet.operation = Cena::BroadcastPacket::ONLINE;
}

[[noreturn]] void LenaClient::broadcast_thread_main() {
	init_packet();
	broadcast_socket = Gio::Socket::create(Gio::SOCKET_FAMILY_IPV4,
										   Gio::SOCKET_TYPE_DATAGRAM,
										   Gio::SocketProtocol::SOCKET_PROTOCOL_UDP);
	broadcast_socket->set_broadcast(true);
	auto inetAddress = Gio::InetAddress::create(LENA_BROADCAST_ADDRESS);
	broadcast_client_side_address = Gio::InetSocketAddress::create(
			inetAddress,
			LENA_CLIENT_SIDE_BROADCAST_PORT
	);
	broadcast_server_side_address = Gio::InetSocketAddress::create(
			inetAddress,
			LENA_SERVER_SIDE_BROADCAST_PORT
	);
	while (true) {
		sleep(BROADCAST_INTERVAL);
		broadcast_packet_mutex.lock();
		broadcast_socket->send_to(broadcast_client_side_address, (const char *) &broadcast_packet,
								  sizeof(Cena::BroadcastPacket));
		broadcast_socket->send_to(broadcast_server_side_address, (const char *) &broadcast_packet,
								  sizeof(Cena::BroadcastPacket));
		broadcast_packet_mutex.unlock();
		Log::v("Sent broadcast packet");
	}
}

inline void to_lower(std::string &str) {
	std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::tolower(c); });
}

template<typename... Args>
inline void println(const Glib::RefPtr<Gio::OutputStream> &output, const char *format, Args...args) {
	static char buffer[4096];
	size_t size = snprintf(buffer, sizeof(buffer), format, args...);
	output->write(buffer, size);
	output->write("\r\n");
}

inline void println(const Glib::RefPtr<Gio::OutputStream> &output, const char *message) {
	output->write(message, strlen(message));
	output->write("\r\n");
}

inline void println(const Glib::RefPtr<Gio::OutputStream> &output) { output->write("\r\n"); }

void LenaClient::file_on_connected(const Glib::RefPtr<Gio::SocketConnection> &connection) {
	static char file_buffer[LENA_FILE_PART_SIZE];

	Log::i("Received file request");
	std::set<std::string> accepted_names, accepted_extensions;
	bool accept_all_names = false, accept_all_extensions = false;
	parser.parse(connection, [&accepted_names, &accepted_extensions, &accept_all_names, &accept_all_extensions](
			std::string &key, std::string &value) {
		to_lower(value);
		if (key == "Accept-Name") {
			if (value == "*") accept_all_names = true;
			else accepted_names.insert(value);
		} else {
			if (value == "*") accept_all_extensions = true;
			else accepted_extensions.insert(value);
		}
	});
	std::vector<Glib::RefPtr<Gio::File>> files;
	auto enumerator = work_dir->enumerate_children();
	Glib::RefPtr<Gio::FileInfo> file_info;
	size_t total_size = 0;
	while ((file_info = enumerator->next_file()).get() != nullptr) {
		auto file = enumerator->get_child(file_info);
		if (Glib::file_test(file->get_path(), Glib::FILE_TEST_IS_DIR)) continue;
		std::string file_name = file_info->get_name();
		size_t index = file_name.rfind('.');
		if (index == -1) continue;
		to_lower(file_name);
		if ((accept_all_names || accepted_names.find(file_name.substr(0, index)) != accepted_names.end()) &&
			(accept_all_extensions ||
			 accepted_extensions.find(file_name.substr(index + 1)) != accepted_extensions.end())) {
			total_size += file_info->get_size();
			files.push_back(file);
		}
	}
	auto output = connection->get_output_stream();
	println(output, "OK");
	println(output, "Files-Count: %lu", files.size());
	println(output, "Total-Length: %lld", total_size);
	println(output);
	for (const auto &file: files) {
		Log::i("Sending %s", file->get_basename().data());
		println(output, "FILE");
		println(output, "File-Name: %s", file->get_basename().data());
		println(output, "Status: 0");
		println(output, "File-Size: %lld", file->query_info()->get_size());
		println(output);
		auto file_input = file->read();
		size_t read;
		while ((read = file_input->read(file_buffer, LENA_FILE_PART_SIZE)) > 0) {
			println(output, "FILEPART");
			println(output, "Status: 0");
			println(output, "Content-Length: %lld", read);
			println(output);
			output->write(file_buffer, read);
		}
		file_input->close();
	}
	Log::i("File request ended");
	connection->close();
}