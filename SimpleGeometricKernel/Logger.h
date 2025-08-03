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
#include <unordered_map>

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


#define LOG(name,level, message) Logger::getInstance().log(#name,level, message, __FILE__, __FUNCTION__, __LINE__)
#define LOG_INFO(name,message) Logger::getInstance().log(#name,LogLevel::INFO, message, __FILE__, __FUNCTION__, __LINE__)
#define LOG_WARNING(name,message) Logger::getInstance().log(#name,LogLevel::WARNING, message, __FILE__, __FUNCTION__, __LINE__)
#define LOG_ERROR(name,message) Logger::getInstance().log(#name,LogLevel::ERRORR, message, __FILE__, __FUNCTION__, __LINE__)

#ifdef _DEBUG
#define LOG_DEBUG(name,message) Logger::getInstance().log(#name,LogLevel::DEBUG, message, __FILE__, __FUNCTION__, __LINE__)
#else
#define LOG_DEBUG(message)
#endif

#define LOG_G(level, message) Logger::getInstance().log("GLOBAL",level, message, __FILE__, __FUNCTION__, __LINE__)
#define LOG_G_INFO(message) Logger::getInstance().log("GLOBAL",LogLevel::INFO, message, __FILE__, __FUNCTION__, __LINE__)
#define LOG_G_WARNING(message) Logger::getInstance().log("GLOBAL",LogLevel::WARNING, message, __FILE__, __FUNCTION__, __LINE__)
#define LOG_G_ERROR(message) Logger::getInstance().log("GLOBAL",LogLevel::ERRORR, message, __FILE__, __FUNCTION__, __LINE__)

#ifdef _DEBUG
#define LOG_G_DEBUG(message) Logger::getInstance().log("GLOBAL",LogLevel::DEBUG, message, __FILE__, __FUNCTION__, __LINE__)
#else
#define LOG_DEBUG(message)
#endif



#define LOG_G_SET_FORMAT(format) Logger::getInstance().setFormatString("GLOBAL",format)
#define LOG_SET_FORMAT(name,format) Logger::getInstance().setFormatString(#name,format)
// %L — log level (DEBUG, INFO, WARN, ERROR).
// %T — time HH:MM:SS.
// %M — message.
// %t — ID of the thread
// %S — file name
// %# — line number 
// %F - function name
// %N - logger name
// \n - 
// Example "[%L] %T (%S: %#) - %M\n" -> "[DEBUG] 01:30:21 (main.cpp: 7) - message"

#define LOG_TIME_START(name) auto start_##name = std::chrono::high_resolution_clock::now()
#define LOG_TIME_END(name) \
    auto end_##name = std::chrono::high_resolution_clock::now(); \
    auto duration_##name = std::chrono::duration_cast<std::chrono::milliseconds>(end_##name - start_##name).count(); \
    LOG_G_INFO("Execution time (" #name "): " + std::to_string(duration_##name) + " ms")


#define LOG_IN_CONSOLE(val) Logger::getInstance().setConsoleOut(val)


extern "C" LOGGER_API std::string toString(LogLevel logLevel);

extern "C" class LOGGER_API Logger {
public:
	static Logger& getInstance();
	void log(const std::string& name,LogLevel logLevel, const std::string& message, const char* file, const char* function, int line);
	void setFormatString(const std::string& name,const std::string& format);
	void setConsoleOut(bool is);
private:
	std::unordered_map<std::string, std::ofstream> outFile;
	std::unordered_map<std::string, std::string> format;
	std::queue<std::tuple<std::string,LogLevel,std::string>> logQueue;
	std::mutex mutex_;
	std::thread logThread;
	std::condition_variable cv;
	bool stop = false;
	bool isConsoleOut = true;

	Logger();
	~Logger();
	void processLogs();
	void createLogDirectory();
	std::string getCurrentTime();
	void writeLog(std::string& name,LogLevel logLevel, std::string& message);
	std::string formatLogMessage(const std::string& name, LogLevel logLevel, const std::string& message, const std::string& file,
												const std::string& function, int line, std::thread::id threadId);
	
};