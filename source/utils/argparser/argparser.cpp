#include "argparser.hpp"
#include <iostream>

namespace utils
{
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
        auto parsed = Arguments();

        // Check if any arguments have been provided at all.
        if (argc == 1)
            throw InvalidArgsException("Please provide a file to display.");

        for (int i = 1; i < argc; i++)
        {
            std::string arg = argv[i];


            // -h 'show help' option.
            if (arg == "-h") parsed.helpFlag = true;
            else if (arg == "--help") parsed.helpFlag = true;

            // -v 'show version' option.
            else if (arg == "-v") parsed.versionFlag = true;
            else if (arg == "--version") parsed.versionFlag = true;

            // -n 'hide line numbers' option.
            else if (arg == "-n") parsed.showLineNums = false;

            // -c [int] 'force column amount' option.
            else if (arg == "-c")
            {
                // Check if anything comes after '-c'.
                if (i+1 == argc)
                    throw InvalidArgsException("Please provide a number after '-c'.");

                else {
                    try {
                        // Convert input to an int.
                        int count = std::stoi(argv[i+1]);
                        parsed.forceColumnCount = count;

                        // Skip next item in array.
                        i++;

                        if (count < 1 || count > 8)
                            throw InvalidArgsException("Please provide a number between 1-8 after '-c'.");
                    }
                    catch (const std::invalid_argument& ia)
                    {
                        std::stringstream ss;
                        ss << "\"" << argv[i+1] << "\" is not a number, please provide a number between 1-8 after '-c'.";
                        throw InvalidArgsException(ss.str());
                    }
                }
            }

            else {
                // Last argument must be file url.
                if (i == argc -1) 
                    parsed.file = argv[argc - 1];

                else throw InvalidArgsException("Malformed command line arguments, use --help for more info.");
            }
        }

        return parsed;
    }
}