#include "hexme.h"

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

hexme::~hexme()
{
    // Set terminal back to normal mode.
    endwin();
}

void hexme::run()
{

}

void hexme::onResize()
{
    // Get terminal resolution.
    this->width = getmaxy(stdscr); 
    this->height = getmaxx(stdscr);
}
