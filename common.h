
#ifndef LENA_COMMON_H
#define LENA_COMMON_H

#define LENA_VERSION "v0.0.1"

#include <functional>

#include <glibmm.h>

const guint16 LENA_CLIENT_SIDE_BROADCAST_PORT = 12574;
const guint16 LENA_SERVER_SIDE_BROADCAST_PORT = 12575;
const guint16 LENA_CLIENT_FILE_PORT = 12574;
const size_t LENA_FILE_PART_SIZE = 16384;

#define LENA_BROADCAST_ADDRESS "255.255.255.255"

namespace Cena {
	typedef int integer_t;
	typedef bool boolean_t;
	typedef char client_id_t[32];
	typedef char name_t[64];
	typedef char string_t[256];
	typedef unsigned int cardinal_t;

	struct ClientInfo {
	public:
		integer_t version;
		client_id_t client_id;
		name_t name;
		string_t work_dir;
		boolean_t auto_launch;
		integer_t status;
		string_t os;
		cardinal_t permissions;
		boolean_t use_computer_name;

		static ClientInfo random();

		static std::function<size_t(const ClientInfo &)> hash_function;
		static std::function<bool(const ClientInfo &, const ClientInfo &)> equals_function;
	};

	struct BroadcastPacket {
		enum operation_t : cardinal_t {
			BROADCAST = 0U, ONLINE = 1U, UPDATE = 2U, OFFLINE = 3U, NAME_CONFLICT = 4U, SHOW_TEST_MESSAGE = 6U
		};
		cardinal_t protocol_version;
		operation_t operation{};
		ClientInfo info;

		BroadcastPacket() : protocol_version(1), info(ClientInfo::random()) {}
	};

	struct FileTransferPacket {
		cardinal_t operation;
		integer_t fileCount;
		string_t fileName;
		integer_t fileSize;
	};
}

#endif //LENA_COMMON_H
