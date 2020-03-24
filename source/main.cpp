
#include "./app/app.h"
#include "./utils/argparser/argparser.h"
#include "./utils/file/file.h"

int main(const int argc, char const **argv)
{
	utils::arguments programArguments;
	utils::file file;

	try {
		programArguments = utils::parseArgs(argc, argv);

		file.open(programArguments.file);

		app hexme(file, programArguments);
		hexme.run();
		hexme.close();
		
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
