#include "app.hpp"
#include "./../command-handler/command-handler.hpp"
#include "./../gui/dialog/dialog-okay.hpp"

/**
 * Pointer to the global app instance, this is used to give various UI elements access to the app
 * instance when they need it and to ensure there is never more than one active app instance.
 */
std::optional<std::shared_ptr<HexMeApp>> appSingleton;

const static int CTRL_D = 4;
const static int ENTER = 13;
const static int ESCAPE = 27;

HexMeApp::HexMeApp(Arguments &a) : args(a)
{
	this->file = std::shared_ptr<File>(new File(args.file));

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

		init_pair(0, -1, -1);					// DEFAULT
		init_pair(1, 0, -1);					// BLACK
		init_pair(2, 1, -1);					// RED
		init_pair(3, 2, -1);					// BLUE
		init_pair(4, 3, -1);					// GREEN
		init_pair(5, 4, -1);					// YELLOW
		init_pair(6, 5, -1);					// PURPLE
		init_pair(7, 6, -1);					// CYAN
		init_pair(8, 7, -1);					// WHITE
		init_pair(9, COLOR_BLACK, COLOR_WHITE); // CURSOR
	}

	// Initial refresh.
	refresh();

	// Setup all UI elements.
	commandPrompt = new gui::textbox();
	hexView = new gui::HexViewer(this->file);

	if (args.forceColumns.value() > 0)
	{
		hexView->maxColumns = args.forceColumns.value();
	}

	// Calculate initial window values.
	hexView->onResize();
	commandPrompt->onResize();

	this->onRefresh();
}

std::shared_ptr<HexMeApp> HexMeApp::create(Arguments &a)
{
	if (appSingleton.has_value()) {
		throw std::runtime_error(
			"There is already an active app instance."
		);
	}
	HexMeApp* appRaw = new HexMeApp(a);
	std::shared_ptr<HexMeApp> app = std::shared_ptr<HexMeApp>(appRaw);
	appSingleton = app;
	return app;
}

HexMeApp &HexMeApp::onRefresh()
{
	move(0,0);
	clrtoeol();

	std::stringstream ss;
	ss << this->file->getName();
	if (this->file->fileType.has_value()) {
		ss << " | " << this->file->fileType.value().first;
	}
	ss << " | " << file->getCursor();
	mvaddstr(0, 0, ss.str().c_str());
    
	refresh();

	// Do inital refresh for all windows.
	hexView->onRefresh();
	commandPrompt->onRefresh();

	return *this;
}

HexMeApp &HexMeApp::onResizeTerminal()
{
	if (getmaxx(stdscr) < 59)
		throw WindowTooSmallException();

	// Update all window content.
	hexView->onResize();
	commandPrompt->onResize();

	// Needed for some reason?
	refresh();

	// Refresh all windows.
	this->onRefresh();
	hexView->onRefresh();
	commandPrompt->onRefresh();

	return *this;
}

void HexMeApp::onHandleInput(const int key_code)
{
	if (key_code == KEY_RESIZE)
	{
		this->onResizeTerminal();
		return ;
	}

	if (key_code == CTRL_D)
	{
		this->close();
		exit(0);
	}

	else if (key_code == ENTER)
	{
		try
		{
			auto cmd = commandPrompt->getText();
			CommandHandler cmdParser(file, this);
			cmdParser.executeCmd(cmd);
			commandPrompt->focus = false;
			commandPrompt->clearText();
		}
		catch (const CmdSyntaxErrorException &error)
		{
			auto msgBox = gui::DialogOkay(error.message);
			msgBox.display();
			commandPrompt->clearText();
		}
	}

	else if (commandPrompt->focus)
	{
		commandPrompt->onInput(key_code);
	}


	else {
		switch (key_code) {
			case KEY_UP:
				if (!file->decCursor(hexView->getColumnCount() * 8)) return;
				break;
			case KEY_DOWN:
				if (!file->incCursor(hexView->getColumnCount() * 8)) return;
				break;
			case KEY_LEFT:
				if (!file->decCursor()) return;
				break;
			case KEY_RIGHT:
				if (!file->incCursor()) return;
				break;
			default:
				commandPrompt->focus = true;
				commandPrompt->onInput(key_code);
				break;
		}
	}

	this->onRefresh();
	hexView->onRefresh();
	commandPrompt->onRefresh();
}

HexMeApp &HexMeApp::run()
{
	int saw_escape = false;
	while (true)
	{
		int key_code = getch();

		if (!saw_escape && key_code == ESCAPE)
		{
			saw_escape = true;
		}

		if (saw_escape)
		{
			if (key_code == 100)
			{
				key_code = gui::textbox::CTRL_DELETE;
			}
		}

		if (saw_escape && key_code != ESCAPE)
		{
			saw_escape = false;
		}

		this->onHandleInput(key_code);
	}

	// Return reference to self.
	return *this;
}

void HexMeApp::close()
{
	// Clear singleton since the user could decide to create a new app instance after this..
	appSingleton.value().reset();
	
	// Set terminal back to normal mode.
	echo();
	endwin();
}

std::shared_ptr<HexMeApp> getApp() {
	if (!appSingleton.has_value()) {
		throw std::runtime_error(
			"Cannot call getApp() before there is an active app instance."
		);
	}
	return appSingleton.value();
}