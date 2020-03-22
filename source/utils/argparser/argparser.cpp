#include "argparser.h"

namespace utils
{
    arguments parseArgs(const int argc, const char **argv)
    {
        auto parsed = arguments();

        // Check if any arguments have been provided at all.
        if (argc == 1)
            throw InvalidArgsException("Please provide a file to display.");

        for (int i = 1; i < argc; i++)
        {
            std::string arg = argv[i];

            // -h 'show help' option.
            if (arg == "-h") parsed.showHelp = true;
            else if (arg == "--help") parsed.showHelp = true;

            // -v 'show version' option.
            else if (arg == "-v") parsed.showVersion = true;
            else if (arg == "--version") parsed.showVersion = true;

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
                        parsed.forceCollumCount = count;

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

                else throw InvalidArgsException("Please provide a file to display.");
            }
        }

        return parsed;
    }
}