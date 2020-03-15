#include "./../../exceptions/exceptions.h"

#include <string>
#include <fstream>

#define byte char

namespace utils
{
    class file
    {
        private:
            std::fstream fileStream;
            unsigned long long cursor = 0;

        public:
            file() {};

            file& open(const char* url);
            file& open(std::string url);

            unsigned long long getCursorLocation() {return cursor;}
            unsigned long long getNextLocation(byte* bytes, int length);

            file& moveCursor(unsigned long long location);
            file& resetCursor();

            file& insertBytes(const byte* bytes, const int length);
            file& deleteBytes(const int length);
            file& replaceBytes(const byte* bytes, const int length);
    };
}
