#include "./app/app.hpp"
#include "./utils/argparser/arguments.hpp"
#include "./utils/argparser/exceptions.hpp"
#include "./utils/file/file.h"

int main(const int argc, char const **argv)
{
	utils::file file;
	HexMeApp* application = nullptr;

	try {
		auto args = Arguments::parse(argc, argv);

		if (args.version.value()) {
			Arguments::printVersion();
			exit(0);
		}

		if (args.help.value()) {
			Arguments::printHelp();
			exit(0);
		}

    	if (args.file == "")
        	throw InvalidArgsException("Please provide a file to display.");

		file.open(args.file);

		application = new HexMeApp(file, args);
		application->run();
		application->close();
		delete application;
		
		file.close();
	}

	catch (const InvalidArgsException &error) {
		std::cerr << "ERROR: " << error.message << std::endl;
		Arguments::printUsage();
		exit(1);
	}

	catch (const utils::FailedToOpenFileException &error) {
		file.close();
		std::cerr << "ERROR: " << error.message << ": \"" << error.path << "\"" << std::endl;
		exit(1);
	}
	
	catch (const HexMeException &error) {
		file.close();
		if (application != nullptr) {
			application->close();
			delete application;
		}
		std::cerr << "ERROR: " << error.message << std::endl;
		exit(1);
	}
}
