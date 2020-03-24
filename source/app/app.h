#pragma once

#include "./../exceptions/exceptions.h"
#include "./cmdparser/cmdparser.h"

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

#define byte char

class app
{
	private:
		gui::textbox* cmdPromt;
		gui::viewer* hexView;

		utils::file& file;
		utils::arguments& args;

	public:
		app(utils::file& File, utils::arguments& Args);

		app& close();
		app& run();
		app& onMoveCursor(int n);
		app& executeCmd(const std::string& cmd);
};
