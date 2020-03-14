#pragma once

#include "./settings/settings.h"
#include "./utils/argparse/argparse.h"

#include <fstream>
#include <sstream>
#include <ncurses.h>

namespace hexme
{
    class app
    {
        private:
            int width, height;
            int selectedX, selectedY;
            std::filebuf& file;
            arguments args;
            settings sets;

        public:
            app(std::filebuf& file, arguments args);

            void close();
            void run();
            void onResize();
    };
}