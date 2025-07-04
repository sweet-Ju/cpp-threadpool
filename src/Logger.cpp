#include "Logger.h"
const char* Logger::s_level[LEVEL_COUNT]
{
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR",
    "FATAL"
};

Logger::~Logger()
{
    if (Logger::getInstance().log_file_.is_open()) {
        Logger::getInstance().log_file_.close();  
    }
}
void Logger::log(LogLevel level, const char* file, int line,const char* func,const std::string& msg)
{
    //auto& logger = Logger::getInstance();
    if (!Logger::getInstance().log_file_.is_open()) {
        if (Logger::getInstance().log_path_.empty()) {
            Logger::getInstance().log_path_ = "default.log";  // 使用默认路径
        }
        Logger::getInstance().log_file_.open(Logger::getInstance().log_path_, std::ios::out | std::ios::app);
        if (Logger::getInstance().log_file_.fail()) {
            throw std::runtime_error("Logger: Failed to open log file: " + Logger::getInstance().log_path_);
        }
    }
    const char* filename = std::strrchr(file, '/');
    filename = filename ? filename + 1 : file;
    std::lock_guard<std::mutex> lock(Logger::getInstance().log_mutex_);
    std::stringstream oss;
    oss << to_time_string() << " [" << std::this_thread::get_id() << "] "
        << s_level[level] <<" "<< "("<<filename<<":"<<line <<" "<<func<<")"<<" "<< msg << "\n";
    Logger::getInstance().log_file_ << oss.str();
    Logger::getInstance().log_file_.flush();
}


std::string Logger::to_time_string()
{
    auto now = std::chrono::system_clock::now();
    time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm localTime;
    localtime_r(&now_time,&localTime);
    char Buffer[32];
    if (strftime(Buffer, sizeof(Buffer), "%F %T", &localTime) > 0) {
        return std::string(Buffer);
    } else {
        return std::string("UNKNOWN_TIME");
    }
}

void Logger::open(const std::string& path)
{
    Logger::getInstance().log_path_ = path;
    Logger::getInstance().log_file_.open(path,std::ios::out|std::ios::app);
    if(Logger::getInstance().log_file_.fail())
    {
        throw std::runtime_error("failed to open file");
    }
     
}
void Logger::close()
{
    Logger::getInstance().log_file_.close();
 
}

Logger &Logger::getInstance()
{
    static Logger instance;
    return instance;
}

void Logger::debug(const std::string &msg)
{
    Logger::getInstance().log(Logger::LogLevel::DEBUG,__FILE__,__LINE__,__func__,msg);
}

void Logger::info(const std::string &msg)
{
    Logger::getInstance().log(Logger::LogLevel::INFO,__FILE__,__LINE__,__func__,msg);
}

void Logger::warn(const std::string &msg)
{
    Logger::getInstance().log(Logger::LogLevel::WARN,__FILE__,__LINE__,__func__,msg);
}

void Logger::error(const std::string &msg)
{
    Logger::getInstance().log(Logger::LogLevel::ERROR,__FILE__,__LINE__,__func__,msg);
}

void Logger::fatal(const std::string &msg)
{
    Logger::getInstance().log(Logger::LogLevel::FATAL,__FILE__,__LINE__,__func__,msg);
}
