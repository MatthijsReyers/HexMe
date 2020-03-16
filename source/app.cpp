#include "app.h"
#include "./settings/settings.h"

#define byte char

app::app(utils::file& File, utils::arguments& Args) : file(File), args(Args)
{
    // Setup nCurses.
    initscr();
    keypad(stdscr, true);
    noecho();

    // Calculate intial window values.
    this->onResize();
}

app& app::run()
{
    this->running = true;
    while (this->running)
    {
        int input = getch();

        if (input == KEY_RESIZE)
            this->onResize();

        else if (input == KEY_RESIZE)
            this->focusCmdPromt = false;

        else if (cmdWindow.hasFocus())
        {
            if (input == KEY_ENTER)
                executeCmd(cmdWindow.getCmd());
            else cmdWindow.onInput(input);
        }

        else switch (input)
        {
            case KEY_DOWN:
                this->onMoveCursor(columns*8);
                break;
            case KEY_UP:
                this->onMoveCursor(columns*-8);
                break;
            case KEY_LEFT:
                this->onMoveCursor(-1);
                break;
            case KEY_RIGHT:
                this->onMoveCursor(1);
            default:
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
    // Get terminal resolution.
    this->width = getmaxy(stdscr); 
    this->height = getmaxx(stdscr);

    // Calculate new row and colum count.
    this->columns = (width - LINENUMBERSWIDTH - 1) / COLUMNWIDTH;
    this->rows = height - 4;

    // Update all windows.
    // TODO

    // Return reference to self.
    return (*this);
}

app& app::onMoveCursor(int n)
{
    // Update all windows.
    // TODO

    // Return reference to self.
    return (*this);
}

app& app::executeCmd(const std::string& cmd)
{
    return (*this);
}