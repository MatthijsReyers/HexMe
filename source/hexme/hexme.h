#include "./../settings/settings.h"
#include "./../argparse/argparse.h"

#include <fstream>
#include <sstream>
#include <ncurses.h>

class hexme
{
    private:
        int width, height;
        int selectedX, selectedY;
        std::fstream file;
        arguments args;
        settings sets;

    public:
        hexme(std::fstream &file, arguments args);
        ~hexme();

        void run();
        void onResize();
};