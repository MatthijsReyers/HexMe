#pragma once

#include "./exceptions.h"
#include "./../gui/textbox/textbox.h"
#include "./../gui/hex-viewer/hex-viewer.hpp"
#include "./../utils/argparser/arguments.hpp"
#include "./../file/file.hpp"

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

		std::shared_ptr<File> file;
		Arguments& args;

		// HexMeApp(const HexMeApp& copy) = delete;
		// HexMeApp& operator=(const HexMeApp&) = delete;

	protected:
		HexMeApp(Arguments& Args);

	public:
		static std::shared_ptr<HexMeApp> create(Arguments& Args);

		void close();
		HexMeApp& run();
		
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