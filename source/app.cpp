#include "app.h"
#include "./settings/settings.h"

#define byte char

namespace hexme
{
    app::app(std::filebuf &File, arguments Args) : file(File), args(Args)
    {
        // Setup nCurses.
        initscr();
        keypad(stdscr, true);
        noecho();

        // Get terminal resolution.
        this->width = getmaxy(stdscr); 
        this->height = getmaxx(stdscr);
    }

    void app::run()
    {
        // settings programSettings = loadSettings();
        getch();

        byte a[4] = {0x34, 0x34, 0x34, 0x34};
        file.sputn((char*)&a, 4);
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
    }

}
