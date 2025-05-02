#include "arguments.hpp"
#include "exceptions.hpp"

#include <iostream>
#include <deque>

#include "flag-argument.hpp"
#include "exceptions.hpp"
#include "arguments.hpp"

Arguments::Arguments() { }

void Arguments::printHelp() {
    Arguments::printVersion();
    Arguments::printUsage();
    std::cout << "Opens the given file in a hex editor." << std::endl << std::endl;
    std::cout << "The following flags can be passed as options:" << std::endl;
    std::cout << " -h, --help         Shows this list" << std::endl;
    std::cout << " -v, --version      Only output the version of the app" << std::endl;
    std::cout << " -n                 Do not display line numbers" << std::endl;
    std::cout << " -c [COLUMNS]       Force the app to show exactly this amount of columns even if" << std::endl;
    std::cout << "                    more columns would fit in the terminal window" << std::endl;
}

void Arguments::printUsage() {
    std::cout << "Usage: hexme (OPTIONS) [FILE]" << std::endl;
}

void Arguments::printVersion() {
    std::cout << "HexMe version " << VERSION << " (c) Matthijs Reyers" << std::endl;
}

Arguments Arguments::parse(const int argc, const char **argv)
{
    if (argc == 0)
        throw InvalidArgsException("Please provide a file to display.");

    auto parsed = Arguments();

    std::deque<std::string> args;
    for (int i = 1; i < argc; i++)
        args.push_back(argv[i]);

    while (args.size() > 0) {
        auto start_size = args.size();
        
        for (auto flag : parsed.flagArgs) {
            flag->checkArgs(args);
        }
        for (auto flag : parsed.intArgs) {
            flag->checkArgs(args);
        }

        // If nothing was popped from the stack that means non of the command line arguments claimed
        // this token and it should be the file url.
        auto current_size = args.size();
        if (start_size == current_size) {
            if (current_size == 1) {
                parsed.file = args.front();
                break;
            } 
            else {
                std::stringstream ss;
                ss << "Unknown argument \"" << args.front() << "\", is this a typo?";
                throw InvalidArgsException(ss.str());
            }
        }
    }

    return parsed;
}