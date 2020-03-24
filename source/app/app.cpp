#include "app.h"

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
	ss << file.getFileName();
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

app& app::run()
{
	while (true)
	{
		// Get user input.
		int input = getch();

		std::stringstream ss;
		ss << input;
		mvaddstr(0,0,ss.str().c_str());
		refresh();

		if (input == KEY_RESIZE)
		{
			// Update all window content.
			hexView->onResize();
			cmdPromt->onResize();

			// Needed for some reason?
			refresh();

			// Refresh all windows.
			hexView->onRefresh();
			cmdPromt->onRefresh();
		}

		else if (input == 10 /*KEY_ENTER*/) {
			auto cmd = cmdPromt->getText();
			cmdparser cmdParser(file,this);
			cmdParser.executeCmd(cmd);
			cmdPromt->clearText();
			hexView->onRefresh();
		}

		else if (cmdPromt->focus) {
			cmdPromt->onInput(input);
			cmdPromt->onRefresh();
		}

		else switch (input)
		{
			case KEY_DOWN:
				hexView->onMoveCursor(hexView->getColumnCount()*8);
				hexView->onRefresh();
				cmdPromt->onRefresh();
				break;
			case KEY_UP:
				hexView->onMoveCursor(hexView->getColumnCount()*-8);
				hexView->onRefresh();
				cmdPromt->onRefresh();
				break;
			case KEY_LEFT:
				hexView->onMoveCursor(-1);
				hexView->onRefresh();
				cmdPromt->onRefresh();
				break;
			case KEY_RIGHT:
				hexView->onMoveCursor(1);
				hexView->onRefresh();
				cmdPromt->onRefresh();
				break;
			default:
				cmdPromt->focus = true;
				cmdPromt->onInput(input);
				cmdPromt->onRefresh();
				break;
		}
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