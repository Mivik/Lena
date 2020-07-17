
#ifndef LENA_REQUEST_PARSER_H
#define LENA_REQUEST_PARSER_H

#include <functional>

#include <giomm.h>

const size_t REQUEST_BUFFER_SIZE = 256;

class RequestParser {
public:
	typedef std::function<void(std::string &, std::string &)> listener_t;

	RequestParser() = default;

	void parse(const Glib::RefPtr<Gio::SocketConnection>&, const listener_t &);

private:
	char buffer[REQUEST_BUFFER_SIZE];
};

#endif //LENA_REQUEST_PARSER_H
