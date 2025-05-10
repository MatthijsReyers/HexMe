#pragma once

#include "dialog.hpp"

#include "./../../app/app.hpp"

#include <ncurses.h>
#include <vector>
#include <string>

namespace gui
{
    class DialogYesNo : public Dialog
    {
    public:
        DialogYesNo(const DialogYesNo&) = delete;
        DialogYesNo& operator=(const DialogYesNo&) = delete;

        DialogYesNo(std::vector<std::string> text);
        DialogYesNo(std::string text);
    };
}