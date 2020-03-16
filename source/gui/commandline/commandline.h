#pragma once

#include <string>
#include <ncurses.h>

namespace gui
{
    class commandline
    {
        private:
            WINDOW* window;
            std::string command = "test";
            bool focus = false;

        public:
            commandline();
            
            commandline& onInput(int i);
            commandline& clear();
            commandline& refresh();
            
            std::string getCmd();
            bool hasFocus();
    };
}