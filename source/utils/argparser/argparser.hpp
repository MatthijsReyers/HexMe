#pragma once

#include <string>
#include <sstream>
#include <stdexcept>

const static char* VERSION = "3.2.0";

namespace utils
{
    class Arguments
    {
        private:
            Arguments() {};

        public:
            static Arguments parse(int argc, const char *argv[]);

            static void printVersion();
            static void printUsage();
            static void printHelp();

            bool helpFlag = false;
            bool versionFlag = false;
            bool showLineNums = true;

            int forceColumnCount = 0;

            std::string file;
    };

    struct InvalidArgsException
    {
        public:
            const std::string message = "";
            
            InvalidArgsException() {}
            InvalidArgsException(const char* msg) : message(std::string(msg)) {}
            InvalidArgsException(const std::string msg) : message(msg) {}
    };
}
