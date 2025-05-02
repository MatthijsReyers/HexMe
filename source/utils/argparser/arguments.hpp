#pragma once

#include "flag-argument.hpp"
#include "int-argument.hpp"

#include <vector>
#include <string>

const static char* VERSION = "3.2.0";

class Arguments
{
    public:
        std::vector<FlagArgument*> flagArgs;
        std::vector<IntArgument*> intArgs;

        Arguments();

        static Arguments parse(int argc, const char *argv[]);

        static void printVersion();
        static void printUsage();
        static void printHelp();

        FlagArgument help = FlagArgument::create<"-h", "--help">(this, "Shows this list", false);
        FlagArgument version = FlagArgument::create<"-v", "--version">(this, "Only output the version of the app", false);
        FlagArgument lineNumbers = FlagArgument::create<"-n">(this, "Do not display line numbers", false);

        // IntArgument forceColumns = IntArgument::create<"-c", "--force-columns">(this, "Force the app to show exactly this amount of columns even if more columns would fit in the terminal window");

        std::string file;
};
