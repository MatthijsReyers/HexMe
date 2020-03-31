#include "logging.h"

void init_log()
{
    logfile.open("log.txt", std::ios::out);
}

void log_message(std::string msg)
{
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    logfile << '[' << ctime(&time);
    logfile.seekp(-1, std::ios_base::cur);
    logfile << "] " << msg << std::endl;
}

template <class T>
void log_message(std::string msg, T num)
{
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    logfile << '[' << ctime(&time);
    logfile.seekp(-1, std::ios_base::cur);
    logfile << "] " << msg << ':' << num << std::endl;
}

void close_log()
{
    logfile.close();
}