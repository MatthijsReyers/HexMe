#include <iostream>

void noFileError()
{
    std::cout << "ERROR: Please specify a file or use -h for help." << std::endl;
    exit(1);
}

void parseArgs(int &argc, char* (&argv)[], int &rows, char* &fileUrl)
{
    // Basic check if a file has been given.
    if (argc == 1) noFileError();


    for (int i = 1; i < argc; i++)
    {

    }
    
}