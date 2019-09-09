#include <iostream>
// #include <stdio.h>
// #include <string.h>

int charToInt(char* inputchar)
{
    std::string input = inputchar;
    if (input == "0") return 0;
    else if (input == "1") return 1;
    else if (input == "2") return 2;
    else if (input == "3") return 3;
    else if (input == "4") return 4;
    else return 5;
}

void showHelp()
{
    std::cout << "HexMe version 1.2" << std::endl;
    std::cout << "-------------------------------------------" << std::endl;
    std::cout << "Usage: hexme [OPTION]... [FILE]..." << std::endl;
    std::cout << std::endl;
    std::cout << "Debug related options:" << std::endl;
    std::cout << "-h, --help        Shows this menu." << std::endl;
    std::cout << "-v, --version     Gives version nr of this installation." << std::endl;
    std::cout << std::endl;
    std::cout << "Program options:" << std::endl;
    std::cout << "-n                Do not show line numbers." << std::endl;
    std::cout << "-c [int]          How many collums to use for listing" << std::endl;
    std::cout << "                  bytes, (default = 2, max = 4)." << std::endl;


    exit(0);
}

void showVersion()
{
    std::cout << "HexMe version 1.2" << std::endl;
    exit(0);
}

void rowsMaxError()
{
    std::cout << "ERROR: The maxium amount of columns is 4." << std::endl;
    exit(1);
}

void rowsMinError()
{
    std::cout << "ERROR: The minimum amount of columns is 1." << std::endl;
    exit(1);
}

void argumentError(char* arg)
{
    std::cout << "ERROR: '" << arg << "' is not a valid argument." << std::endl;
    exit(1);
}

void noFileError()
{
    std::cout << "ERROR: Please specify a file or use -h for help." << std::endl;
    exit(1);
}

void parseArgs(int &argc, char** (&argv), int &rows, char* &fileUrl, bool &showLineNr)
{
    std::string arg;

    // Basic check if a file has been given.
    if (argc == 1) noFileError();

    // Parse arguments.
    for (int i = 1; i < argc; i++)
    {
        arg = argv[i];

        // -h 'help' option
        if (arg == "-h") showHelp();
        else if (arg == "--help") showHelp();

        else if (arg == "-v") showVersion();
        else if (arg == "--version") showVersion();

        // -c 'column amount' options
        else if (arg == "-c")
        {
            // Convert next argument into int.
            rows = charToInt(argv[i+1]);
            if (rows > 4) rowsMaxError();
            if (rows < 1) rowsMinError();

            // Skip next argument to prevent error.
            i++; 
        }

        // -n 'hide line numbers' option
        else if (arg == "-n") showLineNr = false;

        // Last argument if file, thus ignored.
        else if (i == (argc-1)) {}

        // Everthing else is not a valid argument.
        else argumentError(argv[i]);
    }

    // Last argument should always be the file url.
    fileUrl = argv[argc - 1];
}

