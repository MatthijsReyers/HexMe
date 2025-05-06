#include "logs.hpp"

#include <iostream>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <ctime>

void logMessage(const char *msg) {
    auto msg2 = std::string(msg);
    logMessage(msg2);
}

void logMessage(std::string& msg) {
    std::string filename = "hexme.logs";

    std::ofstream outFile(filename, std::ios::app);
    if (!outFile) {
        std::cerr << "Failed to open file.\n";
        exit(1);
    }

    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    outFile << "[" << std::put_time(&tm, "%d-%m-%Y %H:%M:%S") << "] " ;

    outFile << msg << std::endl;

    outFile.close();
}
