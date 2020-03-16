#include "commandline.h"

namespace gui
{
    commandline::commandline()
    {
        // Calculate new size.
        int width = getmaxy(stdscr); 
        int y = getmaxx(stdscr) - 5;

        this->window = newwin(width, 1, y, 0);
        this->refresh();
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
        wclear(window);
        mvwprintw(window, 0, 0, command.c_str());
        wrefresh(window);
        return (*this);
    }

    bool commandline::hasFocus()
    {
        return this->focus;
    }

    std::string commandline::getCmd()
    {
        return this->command;
    }
}