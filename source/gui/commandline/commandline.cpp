#include "commandline.h"

namespace gui
{
    commandline::commandline()
    {
        // Calculate new size.
        int width = getmaxy(stdscr); 
        int y = getmaxx(stdscr) - 1;

        this->window = newwin(width, 1, y, 0);
    }

    commandline& commandline::onInput(int i)
    {
        this->command.append(std::string({char(i), 0x00}));
        this->refresh();
        return (*this);
    }

    commandline& commandline::clear()
    {
        this->command = "";
        this->refresh();
        return (*this);
    }

    commandline& commandline::refresh()
    {
        mvwprintw(this->window, 0, 0, this->command.c_str());
        wrefresh(this->window);
        return (*this);
    }
}