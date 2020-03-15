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

void app::run()
{
    this->running = true;
    while (this->running)
    {
        char input = getch();
        if (input == 'c')
            running = false;
    }
}

void app::close()
{
    // Set terminal back to normal mode.
    echo();
    endwin();
}

void app::onResize()
{
    // Get terminal resolution.
    this->width = getmaxy(stdscr); 
    this->height = getmaxx(stdscr);

    // Calculate new row and colum count.
    this->columns = (width - LINENUMBERSWIDTH - 1) / COLUMNWIDTH;
    this->rows = height - 4;
}
