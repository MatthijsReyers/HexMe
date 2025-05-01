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
        int width, height;
        int x, y;
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