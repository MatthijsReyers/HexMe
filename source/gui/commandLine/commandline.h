#include <string>

namespace gui
{
    class commandline
    {
        private:
            std::string command;

        public:
            commandline& onBackSpace();
            commandline& appendChar(char a);
            commandline& clear();

            std::string getCommand();
    };

    
}