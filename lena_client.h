
#ifndef LENA_LENA_CLIENT_H
#define LENA_LENA_CLIENT_H

#include <thread>
#include <mutex>

#include <gtkmm.h>

#include "request_parser.h"
#include "common.h"

class LenaClient {
public:
	static const unsigned int BROADCAST_INTERVAL = 3; // seconds

	LenaClient();

	std::string get_user_name();

	void set_user_name(const std::string &);

	Glib::RefPtr<Gio::File> get_work_dir();

	void set_work_dir(const Glib::RefPtr<Gio::File> &);

private:
	std::thread broadcast_thread;
	Glib::RefPtr<Gio::InetSocketAddress> broadcast_client_side_address, broadcast_server_side_address;
	Glib::RefPtr<Gio::SocketService> file_socket;
	Glib::RefPtr<Gio::Socket> broadcast_socket;
	Cena::BroadcastPacket broadcast_packet;
	std::mutex broadcast_packet_mutex;
	Glib::RefPtr<Gio::File> work_dir;
	RequestParser parser{};

	[[noreturn]] void broadcast_thread_main();

	void file_on_connected(const Glib::RefPtr<Gio::SocketConnection> &connection);

	void init_packet();
};


#endif //LENA_LENA_CLIENT_H
