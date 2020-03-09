#pragma once

#include <string>

class arguments
{
    public:
        arguments(){};

        bool showError = false;
        bool showHelp = false;
        bool showVersion = false;
        bool showLineNums = true;

        int forceCollumCount = 0;

        std::string file;
        std::string error;
};

arguments parseArgs(int argc, const char *argv[]);
