#pragma once
#include <string>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <regex>
#include <unordered_map>
#include <functional>

#ifdef LOGGERDLL_EXPORTS
#define LOGGER_API __declspec(dllexport)
#else
#define LOGGER_API __declspec(dllimport)
#endif

enum class LogLevel {
	DEBUG = 0,
	INFO,
	WARNING,
	ERRORR,
};


#define LOG(level, message) Logger::getInstance().log(level, message, __FILE__, __FUNCTION__, __LINE__)
#define LOG_INFO(message) Logger::getInstance().log(LogLevel::INFO, message, __FILE__, __FUNCTION__, __LINE__)
#define LOG_WARNING(message) Logger::getInstance().log(LogLevel::WARNING, message, __FILE__, __FUNCTION__, __LINE__)
#define LOG_ERROR(message) Logger::getInstance().log(LogLevel::ERRORR, message, __FILE__, __FUNCTION__, __LINE__)

#ifdef _DEBUG
#define LOG_DEBUG(message) Logger::getInstance().log(LogLevel::DEBUG, message, __FILE__, __FUNCTION__, __LINE__)
#else
#define LOG_DEBUG(message)
#endif

#define LOG_ALL(message) \
    LOG_INFO(message);   \
    LOG_WARNING(message); \
    LOG_ERROR(message);   \
    LOG_DEBUG(message);

#define LOG_SET_FORMAT(format) Logger::getInstance().setFormatString(format)
// %L — log level (DEBUG, INFO, WARN, ERROR).
// %T — time HH:MM:SS.
// %M — message.
// %t — ID of the thread
// %S — file name
// %# — line number 
// %F - function name
// \n - 
// Example "[%L] %T (%S: %#) - %M\n" -> "[DEBUG] 01:30:21 (main.cpp: 7) - message"

#define LOG_TIME_START(name) auto start_##name = std::chrono::high_resolution_clock::now()
#define LOG_TIME_END(name) \
    auto end_##name = std::chrono::high_resolution_clock::now(); \
    auto duration_##name = std::chrono::duration_cast<std::chrono::milliseconds>(end_##name - start_##name).count(); \
    LOG_INFO("Execution time (" #name "): " + std::to_string(duration_##name) + " ms")

#define LOG_IN_ONE_FILE(val) Logger::getInstance().setOneFileOut(val)
#define LOG_IN_CONSOLE(val) Logger::getInstance().setConsoleOut(val)


extern "C" LOGGER_API std::string toString(LogLevel logLevel);

extern "C" class LOGGER_API Logger {
public:
	static Logger& getInstance();
	void log(LogLevel logLevel, const std::string& message, const char* file, const char* function, int line);
	void setFormatString(const std::string& format);
	void setOneFileOut(bool is);
	void setConsoleOut(bool is);
private:
	std::ofstream debugFile;
	std::ofstream infoFile;
	std::ofstream warningFile;
	std::ofstream errorFile;
	std::ofstream unionFile;
	std::queue<std::pair<LogLevel,std::string>> logQueue;
	std::mutex mutex_;
	std::thread logThread;
	std::condition_variable cv;
	bool stop = false;
	bool oneFileOut = false;
	bool isConsoleOut = true;
	std::string formatString = "[%L] %T (%S: %#) - %M\n";
	Logger();
	~Logger();
	void processLogs();
	void createLogDirectory();
	std::string getCurrentTime();
	void writeLog(LogLevel logLevel, std::string& message);
	std::string formatLogMessage(LogLevel logLevel, const std::string& message, const std::string& file,
												const std::string& function, int line, std::thread::id threadId);
	
};