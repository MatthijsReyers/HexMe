#pragma once

#include "./exceptions.h"
#include "./../gui/textbox/textbox.h"
#include "./../gui/hex-viewer/hex-viewer.hpp"
#include "./../utils/argparser/arguments.hpp"
#include "./../file/file.hpp"
#include "../../command-handler/command-handler.hpp"

#include <fstream>
#include <sstream>
#include <memory>
#include <iostream>
#include <stdlib.h>
#include <ncurses.h>
#include <locale.h>

class HexMeApp
{
	private:
		gui::textbox* commandPrompt;
		gui::HexViewer* hexView;

		Arguments& args;

		CommandHandler cmdParser;

		HexMeApp(Arguments& Args);

		bool running = false;
		void run();

	public:
		HexMeApp(const HexMeApp& copy) = delete;
		HexMeApp& operator=(const HexMeApp&) = delete;

		std::shared_ptr<File> file;

		static std::shared_ptr<HexMeApp> create(Arguments& Args);

		void close();
		
		void cleanUp();

		/**
		 * Starts the application, note that this function will not return until the app stops.
		 */
		void start();
		
		HexMeApp& onRefresh();
		HexMeApp& onResizeTerminal();
		void onHandleInput(const int key_code);

		HexMeApp& executeCmd(const std::string& cmd);
};

/**
 * Returns a pointer to the singular app instance, note that this function will throw an error if
 * called before the app is initalized so you should take care when using it outside the TUI code.
*/
std::shared_ptr<HexMeApp> getApp();