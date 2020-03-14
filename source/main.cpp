#include "./exceptions/exceptions.h"
#include "./utils/argparse/argparse.h"
#include "./app.h"

#include <fstream>
#include <iostream>
#include <stdlib.h>

int main(const int argc, char const **argv)
{
    arguments programArguments;
    std::filebuf file;

    try {
        programArguments = parseArgs(argc, argv);

        file.open(programArguments.file, std::ios::binary | std::ios::out | std::ios::app);
        if (!file.is_open()) 
            throw FailedToOpenFileException(programArguments.file, "does the file exist?");

        hexme::app application(file, programArguments);
        application.run();
        application.close();
        
        file.close();
    }

    catch (const InvalidArgsException &error) {
        error.printError();
    }
    
    catch (const HexMeException &error) {
        file.close();
        error.printError();
    }
}
