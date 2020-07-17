
#ifndef LENA_LOG_H
#define LENA_LOG_H

#include <utility>
#include <mutex>

#include <gtkmm.h>

namespace Log {
	inline std::mutex mutex;

	template<typename... Args>
	inline void println(FILE *file, const char *tag, const char *format, Args...args) {
		mutex.lock();
		fprintf(file, "[%s] ", tag);
		fprintf(file, format, args...);
		putchar('\n');
		mutex.unlock();
	}

	inline void v(const char *message) {
		println(stdout, "VERBOSE", "%s", message);
	}

	template<typename... Args>
	inline void v(const char *format, Args...args) {
		println(stdout, "VERBOSE", format, args...);
	}

	inline void d(const char *message) {
		println(stdout, "DEBUG", "%s", message);
	}

	template<typename... Args>
	inline void d(const char *format, Args...args) {
		println(stdout, "DEBUG", format, args...);
	}

	inline void i(const char *message) {
		println(stdout, "INFO", "%s", message);
	}

	template<typename... Args>
	inline void i(const char *format, Args...args) {
		println(stdout, "INFO", format, args...);
	}

	inline void w(const char *message) {
		println(stderr, "WARN", "%s", message);
	}

	template<typename... Args>
	inline void w(const char *format, Args...args) {
		println(stderr, "WARN", format, args...);
	}

	inline void e(const char *message) {
		println(stderr, "ERROR", "%s", message);
	}

	template<typename... Args>
	inline void e(const char *format, Args...args) {
		println(stderr, "ERROR", format, args...);
	}
};

#endif //LENA_LOG_H
