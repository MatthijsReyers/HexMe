#include "app.h"
#include "./../command-handler/command-handler.hpp"
#include "./../gui/msgBox/msgBoxOK.h"

app::app(utils::file& File, utils::arguments& Args) : file(File), args(Args)
{
	// Setup nCurses.
	setlocale(LC_ALL, "");
	initscr();
	keypad(stdscr, true);
	nonl();
	noecho();

	// Setup nCurses colors.
	if (has_colors())
	{
		use_default_colors();
		start_color();
		
		init_pair(0, -1, -1);	// DEFAULT
		init_pair(1, 0, -1);	// BLACK
		init_pair(2, 1, -1);	// RED
		init_pair(3, 2, -1); 	// BLUE
		init_pair(4, 3, -1);	// GREEN
		init_pair(5, 4, -1);	// YELLOW
		init_pair(6, 5, -1);	// PURPLE
		init_pair(7, 6, -1);	// CYAN
		init_pair(8, 7, -1);	// WHITE
		init_pair(9, COLOR_BLACK, COLOR_WHITE); // CURSOR
	}

	// Draw file name and header.
	std::stringstream ss;
	ss << file.getName();
	if (file.getHeader() != "")
		ss << " | " << file.getHeader();
	mvaddstr(0,0,ss.str().c_str());

	// Initial refresh.
	refresh();

	// Setup all UI elements.
	cmdPromt = new gui::textbox();
	hexView = new gui::viewer(this->file);

	// Calculate intial window values.
	hexView->onResize();
	cmdPromt->onResize();

	// Do inital refresh for all windows.
	hexView->onRefresh();
	cmdPromt->onRefresh();
}

app& app::onResizeTerminal() {
	if (getmaxx(stdscr) < 59)
		throw WindowTooSmallException();

	// Update all window content.
	hexView->onResize();
	cmdPromt->onResize();

	// Needed for some reason?
	refresh();

	// Refresh all windows.
	hexView->onRefresh();
	cmdPromt->onRefresh();
	
	return *this;
}

app& app::onHandleInput(const int key_code) {
	if (key_code == KEY_RESIZE) {
		this->onResizeTerminal();
		return *this;
	}

	if (key_code == 13 /* KEY_ENTER */) {
		try {
			auto cmd = cmdPromt->getText();
			CommandHandler cmdParser(file,this);
			cmdParser.executeCmd(cmd);
			hexView->onRefresh();
			cmdPromt->clearText();
			cmdPromt->onRefresh();
			cmdPromt->focus = false;
		}
		catch (const CmdSyntaxErrorException &error) {
			auto fix = gui::msgBoxOK(error.message);
			hexView->onRefresh();
			cmdPromt->clearText();
			cmdPromt->onRefresh();
		}
	}

	else if (cmdPromt->focus) {
		cmdPromt->onInput(key_code);
		cmdPromt->onRefresh();
	}

	else {
		switch (key_code)
		{
			case KEY_UP: 
				file.decCursor(hexView->getColumnCount()*8);
				break;
			case KEY_DOWN:
				file.incCursor(hexView->getColumnCount()*8);
				break;
			case KEY_LEFT:
				file.decCursor();
				break;
			case KEY_RIGHT:
				file.incCursor();
				break;
			default:
				cmdPromt->focus = true;
				cmdPromt->onInput(key_code);
				break;
		}
		hexView->onRefresh();
		cmdPromt->onRefresh();
	}

	return *this;
}

app& app::run()
{
	int saw_escape = false;
	while (true)
	{
		int key_code = getch();

		if (!saw_escape && key_code == 27 /* KEY_ESC */ ){
			saw_escape = true;
		}

		if (saw_escape) {
			if (key_code == 100) {
				key_code = gui::textbox::CTRL_DELETE;
			}
		}

		if (saw_escape && key_code != 27 /* KEY_ESC */ ){
			saw_escape = false;
		} 

		this->onHandleInput(key_code);
	}

	// Return reference to self.
	return *this;
}

app& app::close()
{
	// Set terminal back to normal mode.
	echo();
	endwin();

	// Return reference to self.
	return *this;
}
