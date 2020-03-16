#pragma once

#include "./../../utils/file/file.h"
#include "ncurses.h"

namespace gui
{
    class main
    {
        private:
            int width, height;
            WINDOW* window;
            utils::file file;

            void drawRow(long r);
            void drawBorders();

        public:
            main(utils::file File);
            main& onResize();
            main& refresh();
    };

    main::main(utils::file File) : file(File)
    {
        // Create window
        this->window = newwin(width, height, 1, 0);

        // Initial resize.
        this->onResize();
    }

    void main::drawRow(long r)
    {

    }

    void main::drawBorders()
    {
        // Draw the surrounding box.
        box(this->window, '2', '2');
    }

    main& main::onResize()
    {
        // Clear window contents.
        wclear(this->window);

        // Calculate new size.
        this->width = getmaxy(stdscr); 
        this->height = getmaxx(stdscr) - 2;

        this->drawBorders();

        return (*this);
    }

    main& main::refresh()
    {

    }
}
