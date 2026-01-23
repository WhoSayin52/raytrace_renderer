#include "logger.hpp"

#include <iostream>

#define BLUE "\33[34m"
#define YELLOW "\33[33m"
#define GREEN "\033[32m"
#define RED "\033[31m"
#define RESET "\033[0m"

namespace Logger {

	void info(const char* message) {
		std::cout << GREEN << "INFO: " << message << RESET << '\n';
	}

	void warning(const char* message) {
		std::cout << YELLOW << "WARNING: " << message << RESET << '\n';
	}

	void error(const char* message) {
		std::cout << RED << "ERROR: " << message << RESET << '\n';
	}

	void debug(const char* message) {
		std::cout << BLUE << "DEBUG: " << message << RESET << '\n';
	}

} // namespace Logger
