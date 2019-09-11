// Main headers
#include <curses.h>
#include <iostream>
#include <fstream>

class window
{
    private:
        // Variables
        int winX = 0;
        int winY = 0;

        // Functions
        void resize();
        void render();

    public:
        // Variables
        bool running = true;
};

void window::resize()
{
    printf("screen resized.");
}

void window::render()
{
    // Start ncurses and set up basic settings for window.
    initscr();
    resize();

    // "infinite loop"
    while (running)
    {
        
    }

    // End ncurses.
    endwin();
}