#include "exceptions/exceptions.h"
#include "argparse/argparse.h"
#include "hexme/hexme.h"

#include <fstream>
#include <iostream>
#include <stdlib.h>

int main(const int argc, char const **argv)
{
    arguments programArguments;
    std::fstream file;
    hexme app;

    try {
        programArguments = parseArgs(argc, argv);

        file.open(programArguments.file);
        if (!file.is_open()) 
            throw FailedToOpenFileException(programArguments.file, "does the file exist?");

        app = hexme(file, programArguments);
        app.run();
        app.close();
    }

    catch (InvalidArgsException error) {
        error.printError();
    }
    
    catch (FailedToOpenFileException error) {
        file.close();
        error.printError();
    }

    catch (HexMeException error) {
        app.close();
        file.close();
        error.printError();
    }
}
