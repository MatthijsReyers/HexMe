#pragma once

#include <ncurses.h>

#include <vector>
#include <string>

class msgBox
{
protected:
    int width, height;
    int x, y;

public:
    msgBox(std::vector<std::string> text);
    ~msgBox();

    msgBox& onResize();
    msgBox& onRefresh();
    msgBox& onInput(int input);
};
