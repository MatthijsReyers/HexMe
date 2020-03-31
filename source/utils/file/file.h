#pragma once

#include <string>
#include <fstream>

#define byte char

namespace utils
{
    class file
    {
    private:
        std::fstream* fileStream;
        std::filebuf* fileBuffer;
        std::string path = "";
        std::string header = "";

    public:
        file();
        file(const char* path); 
        file(const std::string& path);

        file& open(const char* path);
        file& open(const std::string& path);

        file& close();

        std::string getPath();
        std::string getName();
        std::string getHeader();

        unsigned long long getFileEnd();
        unsigned long long getCursorLocation();

        file& resetCursor();
        file& moveCursor(unsigned long long location);
        file& incCursor();
        file& incCursor(const unsigned long long n);
        file& decCursor();
        file& decCursor(const unsigned long long n);

        unsigned long long getBytesAfterCursor();

        byte getCurrentByte();
        byte* getCurrentBytesN(const int n);

        file& insertByte(const byte in);
        file& insertBytes(const byte* in, const int n);
        
        file& replaceByte(const byte in);
        file& replaceBytes(const byte* in, const int n);
    };

    struct FailedToOpenFileException
    {
        public:
            std::string message, path;
            FailedToOpenFileException(const std::string msg, const std::string file) : message(msg), path(file) {}
    };
}
