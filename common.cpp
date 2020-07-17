
#include "common.h"

#include <glibmm.h>

namespace Cena {

	ClientInfo ClientInfo::random() {
		ClientInfo info{95};
		Glib::Rand rand;
		std::generate((int *) &info.client_id, (int *) (&info.client_id + 1),
					  [&rand] { return rand.get_int(); });
		return info;
	}

	std::function<size_t(const ClientInfo &)> ClientInfo::hash_function = [](const ClientInfo &info) {
		auto *first = (size_t *) &info.client_id, *last = (size_t *) (&info.client_id + 1);
		size_t ret = 0;
		while (first != last) ret ^= *first++;
		return ret;
	};
	std::function<bool(const ClientInfo &, const ClientInfo &)> ClientInfo::equals_function = [](
			const ClientInfo &first, const ClientInfo &second) {
		return !memcmp(first.client_id, second.client_id, sizeof(client_id_t));
	};

}