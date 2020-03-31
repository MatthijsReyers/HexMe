#pragma once

#include <string>
#include <sstream>
#include <stdexcept>

namespace utils
{
    class arguments
    {
        public:
            arguments(){};

            bool showHelp = false;
            bool showVersion = false;
            bool showLineNums = true;

            int forceCollumCount = 0;

            std::string file;
    };

    arguments parseArgs(int argc, const char *argv[]);

    struct InvalidArgsException
    {
        public:
            const std::string message = "";
            
            InvalidArgsException() {}
            InvalidArgsException(const char* msg) : message(std::string(msg)) {}
            InvalidArgsException(const std::string msg) : message(msg) {}
    };
}