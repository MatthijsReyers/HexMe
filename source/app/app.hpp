#pragma once

#include "./exceptions.h"
#include "./../gui/textbox/textbox.h"
#include "./../gui/viewer/viewer.h"
#include "./../utils/argparser/argparser.h"
#include "./../utils/file/file.h"

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
		gui::viewer* hexView;

		utils::file& file;
		utils::arguments& args;


	public:
		HexMeApp(utils::file& File, utils::arguments& Args);

		HexMeApp& close();
		HexMeApp& run();
		
		HexMeApp& onMoveCursor(int n);
		HexMeApp& onResizeTerminal();
		HexMeApp& onHandleInput(const int key_code);

		HexMeApp& executeCmd(const std::string& cmd);
};
