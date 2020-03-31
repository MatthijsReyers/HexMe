#pragma once

#include <fstream>
#include <string>
#include <ctime>
#include <chrono>

inline std::fstream logfile;

void init_log();

void log_message(std::string msg);

template <class T>
void log_message(std::string msg, T num);

void close_log();
