#include "argparse.h"

#include <string>
#include <stdexcept>

options parseArgs(const int argc, const char **argv)
{
    auto parsed = options();

    // Check if any arguments have been provided at all.
    if (argc == 1) {
        parsed.showError = true;
        parsed.error = "Please provide a file to display.";
    }

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
            if (i+1 == argc) {
                parsed.showError = true;
                parsed.error = "Please provide a number after '-c'.";
            }
            else {
                try {
                    // Convert input to an int.
                    int count = std::stoi(argv[i+1]);
                    parsed.forceCollumCount = count;

                    if (count < 1 || count > 8) {
                        parsed.showError = true;
                        parsed.error = "Please provide a number between 1-8 after '-c'.";
                    }
                }
                catch (const std::invalid_argument& ia)
                {
                    parsed.showError = true;
                    parsed.error = "\"";
                }
            }
        }
    }

    // Last argument must be file url.
    parsed.file = argv[argc - 1];

    return parsed;
}
