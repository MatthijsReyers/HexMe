#pragma once

#include <ncurses.h>

#include <ncurses.h>
#include <vector>
#include <string>

namespace gui
{
    class msgBoxOK
    {
    protected:
        const static int ESCAPE = 27;
        const static int ENTER = 10;

        int width, height;
        int x, y;
        WINDOW* window;
        std::vector<std::string> text;

    public:
        msgBoxOK(std::vector<std::string> text);
        msgBoxOK(std::string text);
        ~msgBoxOK();

        msgBoxOK& onResize();
        msgBoxOK& onRefresh();
    };

    msgBoxOK::msgBoxOK(const std::vector<std::string> t): text(t)
    {
        height = text.size() + 3;
        for (auto line : text)
            if (width < int(line.size()) + 4)
                width = line.size() + 4;

        window = newwin(height, width, 0, 0);
        onResize();

        // Wait for user input.
        getch();
    }

    msgBoxOK::msgBoxOK(const std::string t)
    {
        text = std::vector<std::string>();
        text.push_back(t);

        height = text.size() + 3;
        for (auto line : text)
            if (width < int(line.size()) + 4)
                width = line.size() + 4;

        window = newwin(height, width, 0, 0);
        onResize();

        // Wait for user input.
        getch();
    }

    msgBoxOK::~msgBoxOK()
    {
        delwin(window);
    }

    msgBoxOK& msgBoxOK::onResize()
    {
        x = (getmaxx(stdscr) - 12) / (26+11);
        x = (x * (26+11)) + 12;
        x = (x - width) / 2;

        y = (getmaxy(stdscr) - height) / 2;

        wclear(window);
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

        int x, y;
        getyx(stdscr, y, x);

        for (int x = 1; x < width-1; x++) {
            mvwprintw(window, 0, x, horizontal);
            mvwprintw(window, height-1, x, horizontal);
        }

        for (int y = 1; y < height-1; y++) {
            mvwprintw(window, y, 0, vertical);
            mvwprintw(window, y, width-1, vertical);
        }

        mvwprintw(window, 0, 0, cornerTL);
		mvwprintw(window, 0, width-1, cornerTR);
		mvwprintw(window, height-1, 0, cornerBL);
		mvwprintw(window, height-1, width-1, cornerBR);

        for (std::size_t i = 0; i < text.size(); i++)
            mvwprintw(window, 1+i, 2, text[i].c_str());

        wattron(window, COLOR_PAIR(9));
        mvwprintw(window, height-2, (width / 2)-2, "[OK]");
        wattroff(window, COLOR_PAIR(9));

        move(y,x);

        wrefresh(window);
        return *this;
    }
}