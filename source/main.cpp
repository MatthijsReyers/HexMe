#include "./exceptions/exceptions.h"

#include "./gui/textbox/textbox.h"
#include "./gui/viewer/viewer.h"

#include "./utils/argparse/argparse.h"
#include "./utils/file/file.h"

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
		app& onResize();
		app& onMoveCursor(int n);
		app& executeCmd(const std::string& cmd);
};

app::app(utils::file& File, utils::arguments& Args) : file(File), args(Args)
{
	// Setup nCurses.
	setlocale(LC_ALL, "");
	initscr();
	keypad(stdscr, true);
	noecho();

	// Setup nCurses colors.
	if (has_colors())
	{
		use_default_colors();
		start_color();
		
		init_pair(0, -1, -1);	// DEFAULT
		init_pair(1, 1, -1);	// RED
		init_pair(2, 4, -1); 	// BLUE
		init_pair(3, 2, -1);	// GREEN
		init_pair(4, 2, -1);	// GREEN
		init_pair(5, COLOR_BLACK, COLOR_WHITE); // CURSOR
	}

	// Initial refresh.
	refresh();

	// Setup all UI elements.
	cmdPromt = new gui::textbox();
	hexView = new gui::viewer(this->file);

	// Calculate intial window values.
	this->onResize();
}

app& app::run()
{
	bool running = true;
	while (running)
	{
		int input = getch();
		if (input == KEY_RESIZE)
			this->onResize();

		else if (cmdPromt->hasFocus())
		{
			if (input == 10 /*KEY_ENTER*/) {
				this->executeCmd(cmdPromt->getText());
				cmdPromt->clearText();
			}
			else cmdPromt->onInput(input);
		}

		else switch (input)
		{
			case KEY_DOWN:
				this->onMoveCursor(hexView->getColumnCount()*8);
				break;
			case KEY_UP:
				this->onMoveCursor(hexView->getColumnCount()*-8);
				break;
			case KEY_LEFT:
				this->onMoveCursor(-1);
				break;
			case KEY_RIGHT:
				this->onMoveCursor(1);
			default:
				cmdPromt->setFocus(true);
				cmdPromt->onInput(input);
				break;
		}
	}

	// Return reference to self.
	return (*this);
}

app& app::close()
{
	// Set terminal back to normal mode.
	echo();
	endwin();

	// Return reference to self.
	return (*this);
}

app& app::onResize()
{
	// Update all window content.
	hexView->onResize();
	cmdPromt->onResize();

	// Needed for some reason?
	refresh();

	// Refresh all windows.
	hexView->onRefresh();
	cmdPromt->onRefresh();

	// Return reference to self.
	return (*this);
}

app& app::onMoveCursor(int n)
{
	// Update all windows.
	hexView->onRefresh();
	cmdPromt->onRefresh();

	// Return reference to self.
	return (*this);
}

app& app::executeCmd(const std::string& cmd)
{
	return (*this);
}

int main(const int argc, char const **argv)
{
	utils::arguments programArguments;
	utils::file file;

	try {
		programArguments = utils::parseArgs(argc, argv);

		file.open(programArguments.file);

		app hexme(file, programArguments);
		hexme.run();
		hexme.close();
		
		file.close();
	}

	catch (const InvalidArgsException &error) {
		error.printError();
	}
	
	catch (const HexMeException &error) {
		file.close();
		error.printError();
	}
}
