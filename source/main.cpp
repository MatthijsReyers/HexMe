
#include "./app/app.hpp"
#include "./utils/argparser/argparser.hpp"
#include "./utils/file/file.h"

int main(const int argc, char const **argv)
{
	utils::file file;
	HexMeApp* application = nullptr;

	try {
		auto args = utils::Arguments::parse(argc, argv);

		if (args.versionFlag) {
			utils::Arguments::printVersion();
			exit(0);
		}

		if (args.helpFlag) {
			utils::Arguments::printHelp();
			exit(0);
		}


		file.open(args.file);

		application = new HexMeApp(file, args);
		application->run();
		application->close();
		delete application;
		
		file.close();
	}

	catch (const utils::InvalidArgsException &error) {
		std::cout << "ERROR: " << error.message << std::endl;
		utils::Arguments::printUsage();
	}

	catch (const utils::FailedToOpenFileException &error) {
		file.close();
		std::cout << "ERROR: " << error.message << ": \"" << error.path << "\"" << std::endl;
	}
	
	catch (const HexMeException &error) {
		file.close();
		if (application != nullptr) {
			application->close();
			delete application;
		}
		std::cout << "ERROR: " << error.message << std::endl;
	}
}
