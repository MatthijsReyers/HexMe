#include <iostream>

void showHelp()
{
    exit(0);
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

void parseArgs(int &argc, char** (&argv), int &rows, char* &fileUrl)
{
    // Basic check if a file has been given.
    if (argc == 1) noFileError();

    // Parse arguments.
    for (int i = 1; i < (argc - 1); i++)
    {
        // -h 'help' option
        if (argv[i] == "-h") showHelp();

        // -c 'column amount' options
        else if (argv[i] == "-c")
        {
            
            // Skip next argument to prevent error.
            i++;
        }

        // -n 'show line numbers' option
        else if (argv[i] == "-n")
        {
            // Skip next argument to prevent error.
            i++;
        }

        // Everthing else is not a valid argument.
        else argumentError(argv[i]);
    }

    // Last argument should always be the file url.
    fileUrl = argv[0];
}