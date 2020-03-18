#pragma once

#include "./../../exceptions/exceptions.h"

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
        std::string url = "";

    public:
        file();
        file(const char* url); 
        file(const std::string& url);

        file& open(const char* url);
        file& open(const std::string& url);

        file& close();

        std::string getFileName();

        unsigned long long getFileStart();
        unsigned long long getFileEnd();
        unsigned long long getCursorLocation();

        file& resetCursor();
        file& moveCursor(unsigned long long location);
        file& incCursor();
        file& incCursor(int n);
        file& decCursor();
        file& decCursor(int n);

        unsigned long long getBytesAfterCursor();

        byte getCurrentByte();
        // byte* getCurrentBytesN(const int n);

        file& insertByte(const byte in);
        file& insertBytes(const byte* in, const int n);
        
        file& replaceByte(const byte in);
        file& replaceBytes(const byte* in, const int n);
    };
}
