#pragma once

#include <string>

class options
{
    public:
        options(){};

        bool showError = false;
        bool showHelp = false;
        bool showVersion = false;
        bool showLineNums = true;

        int forceCollumCount = 0;

        std::string file;
        std::string error;
};

options parseArgs(int argc, const char *argv[]);
