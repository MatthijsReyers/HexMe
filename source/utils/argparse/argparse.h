#pragma once

#include "./../../exceptions/exceptions.h"

#include <string>
#include <sstream>
#include <stdexcept>

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
