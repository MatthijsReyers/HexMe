#include "textbox.h"

namespace gui
{
	textbox::textbox()
	{
		int y = getmaxy(stdscr) - 1;
		int width = getmaxx(stdscr) - 2;
		window = newwin(1, width, y, 0);
		history.push_back("");

		this->onRefresh();
	}

	textbox::~textbox()
	{
		delwin(window);
	}

	textbox& textbox::onRefresh()
	{
		mvwprintw(window, 0, 0, " > ");
		mvwprintw(window, 0, 3, history[historyPosition].c_str());
		wmove(window, 0, 3+cursorPosition);
		wrefresh(window);
		return *this;
	}

	textbox& textbox::onInput(int key)
	{
		if (key >= 32 && key <= 126) {
			history[0].insert(cursorPosition,{char(key)});
			cursorPosition++;
		}

		else switch (key)
		{
			case ESCAPE:
				this->focus = false;
				break;

			case KEY_UP:
				if (historyPosition < history.size()-1) {
					historyPosition++;
					cursorPosition = history[historyPosition].length();
					wclear(window);
					this->onRefresh();
				} break;

			case KEY_DOWN:
				if (historyPosition > 0) {
					historyPosition--;
					cursorPosition = history[historyPosition].length();
					wclear(window);
					this->onRefresh();
				} break;

			case BACKSPACE:
				if (history[historyPosition].length() > 0 && cursorPosition > 0) {
					history[historyPosition].erase(cursorPosition-1, 1);
					if (cursorPosition > 0)
						cursorPosition--;
				} 
				wclear(window);
				break;

			case CRTL_BACKSPACE:
				while (history[historyPosition].length() > 0 && cursorPosition > 0 && history[historyPosition][cursorPosition-1] != ' ') {
					history[historyPosition].erase(cursorPosition-1, 1);
					if (cursorPosition > 0)
						cursorPosition--;
				}
				while (history[historyPosition].length() > 0 && cursorPosition > 0 && history[historyPosition][cursorPosition-1] == ' ') {
					history[historyPosition].erase(cursorPosition-1, 1);
					if (cursorPosition > 0)
						cursorPosition--;
				} 
				wclear(window);
				break;

			case DELETE:
				if (history[historyPosition].length() > 0 && cursorPosition < history[historyPosition].length())
					history[historyPosition].erase(cursorPosition, 1);
				wclear(window);
				break;

			case KEY_HOME:
				cursorPosition = 0;
				break;

			case KEY_END:
				cursorPosition = history[historyPosition].length();
				break;
			
			case KEY_LEFT:
				if (cursorPosition > 0)
					cursorPosition--;
				break;
			
			case KEY_RIGHT:
				if (cursorPosition < history[historyPosition].size())
					cursorPosition++;
				break;
		}
		return *this;
	}

	textbox& textbox::onResize()
	{
		int y = getmaxy(stdscr) - 1;
		int width = getmaxx(stdscr) - 2;
		wresize(window, 1, width);
		mvwin(window, y, 0);
		return *this;
	}

	std::string& textbox::getText()
	{
		return this->history[historyPosition];
	}

	textbox& textbox::setText(const std::string t)
	{
		this->history[historyPosition] = t;
		return *this;
	}

	textbox& textbox::setText(const char* t)
	{
		this->history[historyPosition] = std::string(t);
		return *this;
	}

	textbox& textbox::clearText()
	{
		if (historyPosition > 0)
			history[0] = history[historyPosition];

		historyPosition = 0;
		cursorPosition = 0;
		history.insert(history.begin(), "");

		if (history.size() > MAX_HISTORY_SIZE)
			history.pop_back();
		
		wclear(window);
		this->onRefresh();
		
		return *this;
	}
}
