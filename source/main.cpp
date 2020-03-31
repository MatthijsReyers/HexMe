
#include "./app/app.h"
#include "./utils/argparser/argparser.h"
#include "./utils/file/file.h"

int main(const int argc, char const **argv)
{
	utils::arguments programArguments;
	utils::file file;
	app* application;

	try {
		programArguments = utils::parseArgs(argc, argv);

		file.open(programArguments.file);

		application = new app(file, programArguments);
		application->run();
		application->close();
		delete application;
		
		file.close();
	}

	catch (const utils::InvalidArgsException &error) {
		std::cout << "ERROR: " << error.message << std::endl;
	}

	catch (const utils::FailedToOpenFileException &error) {
		file.close();
		std::cout << "ERROR: " << error.message << ": \"" << error.path << "\"" << std::endl;
	}
	
	catch (const HexMeException &error) {
		file.close();
		application->close();
		std::cout << "ERROR: " << error.message << std::endl;
	}
}
