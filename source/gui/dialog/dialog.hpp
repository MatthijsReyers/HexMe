#pragma once

#include "./../../app/app.hpp"

#include <ncurses.h>
#include <vector>
#include <string>

namespace gui
{
    class Dialog
    {
    protected:
        unsigned int width = 0;
        unsigned int height = 0;

        unsigned int x = 0;
        unsigned int y = 0;

        size_t choice = 0;

        WINDOW* window;

        Dialog& onResize();
        Dialog& onRefresh();
        Dialog& onInput();

        void drawBorder();
        void drawButtons();

    public:
        /**
         * Text contents of the message box, note that every line 
         */
        std::vector<std::string> text;

        /**
         * Text of the button options
         */
        std::vector<std::string> buttons;

        /**
         * Should the user be forced to make a choice or are they allowed to press "escape" to
         * close the message box without explicitly choosing one of the options/buttons?
         */
        bool forceChoice = false;

        /**
         * The default button choice that is selected when the dialog box first opens.
         */
        size_t defaultChoice = 0;

        Dialog(const Dialog&) = delete;
        Dialog& operator=(const Dialog&) = delete;

        Dialog(std::vector<std::string> text, std::vector<std::string> buttons);
        Dialog(std::vector<std::string> text, std::string button);
        Dialog(std::string text, std::vector<std::string> buttons);
        Dialog(std::string text, std::string button);
        ~Dialog();

        /**
         * Displays the message box on top of whatever UI is currently active. The returned value
         * is the index of the button chosen by the user.
         */
        std::optional<size_t> display();
    };
}