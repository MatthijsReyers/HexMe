#include "./app/app.hpp"
#include "./utils/argparser/arguments.hpp"
#include "./utils/argparser/exceptions.hpp"
#include "./file/file.hpp"

int main(const int argc, char const **argv)
{
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

		auto application = HexMeApp::create(args);
		application->run();
		application->close();
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
		auto application = getApp();
		application->close();
		std::cerr << "ERROR: " << error.message << std::endl;
		exit(1);
	}
}
