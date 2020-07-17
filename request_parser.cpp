
#include "request_parser.h"

void RequestParser::parse(const Glib::RefPtr<Gio::SocketConnection> &connection, const listener_t &listener) {
	auto input = connection->get_input_stream();
	char *begin = nullptr, *end = nullptr;
	std::string key, value;
	bool reading_value = false;
	bool last_is_end_line = false;
	while (true) {
		if (begin == end) {
			begin = buffer;
			end = begin + input->read(buffer, REQUEST_BUFFER_SIZE);
			if (begin == end) break;
		}
		const char c = *begin++;
		if (c == '\n') {
			if (last_is_end_line) break;
			else last_is_end_line = true;
		} else if (c != '\r') last_is_end_line = false;
		switch (c) {
			case '\r':
			case '\n': {
				// trim
				size_t lst = key.size();
				while (!key.empty() && isspace(key[lst - 1])) --lst;
				key.resize(lst);
				lst = value.size();
				while (!value.empty() && isspace(value[lst - 1])) --lst;
				value.resize(lst);
				if (!key.empty() && !value.empty()) listener(key, value);
				key.clear();
				value.clear();
				reading_value = false;
				break;
			}
			case ':': {
				reading_value = true;
				break;
			}
			default: {
				if (reading_value) {
					if (value.empty() && isspace(c)) break;
					value += c;
				} else {
					if (key.empty() && isspace(c)) break;
					key += c;
				}
				break;
			}
		}
	}
}