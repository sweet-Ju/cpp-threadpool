#ifndef LOGGER_H
#define LOGGER_H
#include <iostream>
#include <mutex>
#include <thread>
#include <fstream>
#include <chrono>
#include <sstream> 
#include <cstring>
#define LOG_DEBUG(msg) Logger::getInstance().log(Logger::DEBUG, __FILE__, __LINE__, __func__,msg )
#define LOG_INFO(msg)  Logger::getInstance().log(Logger::INFO,  __FILE__, __LINE__, __func__,msg)
#define LOG_WARN(msg)  Logger::getInstance().log(Logger::WARN,  __FILE__, __LINE__, __func__,msg)
#define LOG_ERROR(msg) Logger::getInstance().log(Logger::ERROR, __FILE__, __LINE__, __func__,msg)
#define LOG_FATAL(msg) Logger::getInstance().log(Logger::FATAL, __FILE__, __LINE__, __func__,msg)
class Logger
{
public:
    enum LogLevel
    {
       DEBUG = 0,
	   INFO,
	   WARN,
	   ERROR,
	   FATAL,
       LEVEL_COUNT
    };
private:
    std::fstream log_file_;
    std::mutex log_mutex_; 
    bool initialized_ = false;
    std::string log_path_;
    LogLevel currentLevel = LogLevel::DEBUG;
private:
    Logger()= default;
    ~Logger();
    Logger(const Logger&)=delete;
    Logger& operator=(const Logger&)=delete;
    static const char * s_level[LEVEL_COUNT];
    static std::string to_time_string();
public:
    static void log(LogLevel level, const char* file, int line,const char* func,const std::string& msg);
    static Logger& getInstance();
    static void open(const std::string& path);
    static void close();
    static void debug(const std::string& msg);
    static void info(const std::string& msg);
    static void warn(const std::string& msg);
    static void error(const std::string& msg);
    static void fatal(const std::string& msg);
};
#endif //LOGGRE_H