#include "app.hpp"
#include "./../command-handler/command-handler.hpp"
#include "./../gui/message-box/message-box-okay.hpp"

HexMeApp::HexMeApp(utils::file& File, utils::arguments& Args) : file(File), args(Args)
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
	commandPrompt = new gui::textbox();
	hexView = new gui::viewer(this->file);

	// Calculate initial window values.
	hexView->onResize();
	commandPrompt->onResize();

	// Do inital refresh for all windows.
	hexView->onRefresh();
	commandPrompt->onRefresh();
}

HexMeApp& HexMeApp::onResizeTerminal() {
	if (getmaxx(stdscr) < 59)
		throw WindowTooSmallException();

	// Update all window content.
	hexView->onResize();
	commandPrompt->onResize();

	// Needed for some reason?
	refresh();

	// Refresh all windows.
	hexView->onRefresh();
	commandPrompt->onRefresh();
	
	return *this;
}

HexMeApp& HexMeApp::onHandleInput(const int key_code) {
	if (key_code == KEY_RESIZE) {
		this->onResizeTerminal();
		return *this;
	}

	if (key_code == 13 /* KEY_ENTER */) {
		try {
			auto cmd = commandPrompt->getText();
			CommandHandler cmdParser(file,this);
			cmdParser.executeCmd(cmd);
			hexView->onRefresh();
			commandPrompt->clearText();
			commandPrompt->onRefresh();
			commandPrompt->focus = false;
		}
		catch (const CmdSyntaxErrorException &error) {
			auto msgBox = gui::MessageBoxOkay(this, error.message);
			msgBox.display();
			hexView->onRefresh();
			commandPrompt->clearText();
			commandPrompt->onRefresh();
		}
	}

	else if (commandPrompt->focus) {
		commandPrompt->onInput(key_code);
		commandPrompt->onRefresh();
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
				commandPrompt->focus = true;
				commandPrompt->onInput(key_code);
				break;
		}
		hexView->onRefresh();
		commandPrompt->onRefresh();
	}

	return *this;
}

HexMeApp& HexMeApp::run()
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

HexMeApp& HexMeApp::close()
{
	// Set terminal back to normal mode.
	echo();
	endwin();

	// Return reference to self.
	return *this;
}
