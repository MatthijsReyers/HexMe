#include "settings/settings.h"
#include "argparse/argparse.h"
#include "hdetect/hdetect.h"
#include "hexme/hexme.h"

#include <fstream>
#include <iostream>
#include <stdlib.h>

int main(const int argc, char const **argv)
{
    arguments programArguments = parseArgs(argc, argv);

    if (programArguments.showError) {
        std::cout << "ERROR: " << programArguments.error << std::endl;
        return 1;
    }

    std::fstream file(programArguments.file);
    if (!file) {
        std::cout << "ERROR: \"" << programArguments.file << "\" is not a file in this directory." << std::endl;
        return 1;
    }

    hexme app = hexme(file, programArguments);
    app.run();

    std::cout << getFileHeaderType(file) << std::endl;

    file.close();
}
