#include "hexme.h"
#include "../settings/settings.h"

hexme::hexme(std::fstream &file, arguments args)
{
    // Setup nCurses.
    initscr();
    keypad(stdscr, true);
    noecho();

    // Get terminal resolution.
    this->width = getmaxy(stdscr); 
    this->height = getmaxx(stdscr);
}

void hexme::run()
{
    settings programSettings = loadSettings();
}

void hexme::close()
{
    // Set terminal back to normal mode.
    echo();
    endwin();
}

void hexme::onResize()
{
    // Get terminal resolution.
    this->width = getmaxy(stdscr); 
    this->height = getmaxx(stdscr);
}
