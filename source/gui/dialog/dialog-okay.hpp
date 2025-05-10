#pragma once

#include "dialog.hpp"

#include "./../../app/app.hpp"

#include <ncurses.h>
#include <vector>
#include <string>

namespace gui
{
    class DialogOkay : public Dialog
    {
    public:
        DialogOkay(const DialogOkay&) = delete;
        DialogOkay& operator=(const DialogOkay&) = delete;

        DialogOkay(std::vector<std::string> text);
        DialogOkay(std::string text);
    };
}