#ifndef LOGGER_HPP
#define LOGGER_HPP

// TODO prevent vs code formatting macros
// TODO add ability for comma separated text description, formating, and output destination

#define LOG_INFO(message) Logger::info((message))
#define LOG_WARNING(message) Logger::warning((message))
#define LOG_ERROR(message) Logger::error((message))
#define LOG_DEBUG(message) Logger::debug((message))

namespace Logger {

	void info(const char* message);
	void warning(const char* message);
	void error(const char* message);
	void debug(const char* message);

} // namespace Logger




#endif // LOGGER_HPP
