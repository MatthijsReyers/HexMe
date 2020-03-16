#pragma once

#include "./../../utils/file/file.h"
#include "ncurses.h"

namespace gui
{
    class hexviewer
    {
        private:
            int width, height;
            WINDOW* window;
            utils::file file;

            void drawRow(long r);
            void drawBorders();

        public:
            hexviewer(utils::file File);
            hexviewer& onResize();
            hexviewer& refresh();
    };

    hexviewer::hexviewer(utils::file File) : file(File)
    {
        // Create window
        this->window = newwin(width, height, 1, 0);

        // Initial resize.
        this->onResize();
    }

    void hexviewer::drawRow(long r)
    {

    }

    void hexviewer::drawBorders()
    {
        // Draw the surrounding box.
        box(this->window, '2', '2');
    }

    hexviewer& hexviewer::onResize()
    {
        // Clear window contents.
        wclear(this->window);

        // Calculate new size.
        this->width = getmaxy(stdscr); 
        this->height = getmaxx(stdscr) - 2;

        this->drawBorders();

        return (*this);
    }

    hexviewer& hexviewer::refresh()
    {

    }
}
