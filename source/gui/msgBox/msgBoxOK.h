#pragma once

#include <ncurses.h>

#include <vector>
#include <string>

namespace gui
{
    class msgBoxOK
    {
    protected:
        int width, height;
        int x, y;
        WINDOW* window;
        std::vector<std::string> text;

    public:
        msgBoxOK(std::vector<std::string> text);
        ~msgBoxOK();

        msgBoxOK& onResize();
        msgBoxOK& onRefresh();
        msgBoxOK& onInput(int input);
    };

    msgBoxOK::msgBoxOK(std::vector<std::string> t): text(t)
    {
        height = text.size() + 3;
        for (auto line : text)
            if (width < line.size() + 4)
                width = line.size() + 4;

        window = newwin(height, width, 0, 0);
    }

    msgBoxOK& msgBoxOK::onResize()
    {
        wclear(window);
        y = (getmaxy(stdscr) - height) / 2;
        x = (getmaxx(stdscr) - height) / 2;
        mvwin(window, y, x);

        onRefresh();

        return *this;
    }

    msgBoxOK& msgBoxOK::onRefresh()
    {
        const static char* horizontal = "═";
		const static char* vertical = "║";

		const static char* cornerTL = "╔";
		const static char* cornerTR = "╗";
		const static char* cornerBL = "╚";
		const static char* cornerBR = "╝";

        for (int x = 1; x < width-2; x++) {
            mvwprintw(window, 0, x, horizontal);
            mvwprintw(window, height-1, x, horizontal);
        }

        for (int y = 1; y < height-2; y++) {
            mvwprintw(window, y, 0, vertical);
            mvwprintw(window, y, width-1, vertical);
        }
        
        wrefresh(window);
        return *this;
    }
}