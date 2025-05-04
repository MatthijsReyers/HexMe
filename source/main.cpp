#include "./app/app.hpp"
#include "./utils/argparser/arguments.hpp"
#include "./utils/argparser/exceptions.hpp"
#include "./file/file.hpp"

int main(const int argc, char const **argv)
{
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

		application = new HexMeApp(args);
		application->run();
		application->close();
		delete application;
	}

	catch (const InvalidArgsException &error) {
		std::cerr << "ERROR: " << error.message << std::endl;
		Arguments::printUsage();
		exit(1);
	}

	catch (const FailedToOpenFileException &error) {
		std::cerr << "ERROR: " << error.message << ": \"" << error.path << "\"" << std::endl;
		exit(1);
	}
	
	catch (const HexMeException &error) {
		if (application != nullptr) {
			application->close();
			delete application;
		}
		std::cerr << "ERROR: " << error.message << std::endl;
		exit(1);
	}
}
