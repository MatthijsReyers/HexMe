#pragma once

#include "./../../app/app.hpp"

#include <ncurses.h>
#include <vector>
#include <string>

namespace gui
{
    class MessageBoxOkay
    {
    protected:
        unsigned int width = 0;
        unsigned int height = 0;

        unsigned int x = 0;
        unsigned int y = 0;
        
        WINDOW* window;
        std::vector<std::string> text;

        MessageBoxOkay& onResize();
        MessageBoxOkay& onRefresh();

        HexMeApp* app;

    public:
        MessageBoxOkay(HexMeApp* app, std::vector<std::string> text);
        MessageBoxOkay(HexMeApp* app, std::string text);
        ~MessageBoxOkay();

        MessageBoxOkay& display();
    };
}