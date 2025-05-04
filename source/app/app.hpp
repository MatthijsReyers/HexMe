#pragma once

#include "./exceptions.h"
#include "./../gui/textbox/textbox.h"
#include "./../gui/hex-viewer/hex-viewer.hpp"
#include "./../utils/argparser/arguments.hpp"
#include "./../file/file.hpp"

#include <fstream>
#include <sstream>
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

		HexMeApp(const HexMeApp& copy) = delete;

	public:
		HexMeApp(Arguments& Args);

		HexMeApp& close();
		HexMeApp& run();
		
		HexMeApp& onRefresh();
		HexMeApp& onResizeTerminal();
		void onHandleInput(const int key_code);

		HexMeApp& executeCmd(const std::string& cmd);
};
