#include "file.h"

#include "./../../utils/hdetect/hdetect.h"
#include <unistd.h>

namespace utils
{
    file::file()
    {
        fileStream = new std::fstream();
    }

    file::file(const char* url)
    {
        this->fileStream = new std::fstream();
        this->open(std::string(url));
    }

    file::file(const std::string& url)
    {
        this->fileStream = new std::fstream();
        this->open(url);
    }

    file& file::open(const char* url)
    {
        return this->open(std::string(url));
    }

    file& file::open(const std::string& url)
    {
        this->url = url;
        this->fileStream->open(url, std::ios::out | std::ios::binary | std::ios::in);
        this->fileBuffer = fileStream->rdbuf();
        this->header = utils::getFileHeaderType(*this);
        return *this;
    }

    file& file::close()
    {
        this->fileStream->close();
        this->fileBuffer->close();
        return *this;
    }

    file& file::save()
    {
        this->fileBuffer->pubsync();
        return *this;
    }

    std::string file::getFileName()
    {
        std::size_t found = url.find_last_of("/\\");
        return url.substr(0,found);
    }

    std::string file::getHeader()
    {
        return header;
    }

    unsigned long long file::getFileEnd()
    {
        auto cursor = getCursorLocation();
        moveCursor(0);
        auto res = getBytesAfterCursor() - 1;
        moveCursor(cursor);
        return res;
    }

    unsigned long long file::getCursorLocation()
    {
        return fileStream->tellg();
    }

    file& file::resetCursor()
    {
        fileBuffer->pubseekpos(0);
        fileStream->seekg(0);
        fileStream->seekp(0);
        return *this;
    }

    file& file::moveCursor(unsigned long long location)
    {
        fileBuffer->pubseekpos(location);
        fileStream->seekg(location);
        fileStream->seekp(location);
        return *this;
    }

    file& file::incCursor()
    {
        moveCursor(getCursorLocation()+1);
        return *this;
    }

    file& file::decCursor()
    {
        auto cursor = getCursorLocation();
        if (cursor != 0)
            moveCursor(cursor-1);
        return *this;
    }

    unsigned long long file::getBytesAfterCursor()
    {
        return fileBuffer->in_avail();
    }

    byte file::getCurrentByte()
    {
        auto res = fileBuffer->sbumpc();
        fileBuffer->sungetc();
        return res;
    }

    file& file::insertByte(const byte newByte)
    {
        auto cursor = getCursorLocation();
        auto end = getFileEnd();
        
        // Place temporary new byte at file end to make room.
        moveCursor(end+1);
        fileBuffer->sputc('=');

        // Shift over all bytes in front of cursor to make room for new byte.
        for (unsigned long long i = end+1; i > cursor; i--) {
            moveCursor(i-1);
            byte current = getCurrentByte();
            moveCursor(i);
            fileBuffer->sputc(current);
        }

        // Move cursor back and finnally insert byte.
        moveCursor(cursor);
        replaceByte(newByte);

        // Return reference to self.
        return *this;
    }

    file& file::insertBytes(const byte* bytes, const int n)
    {
        auto cursor = getCursorLocation();
        auto end = getFileEnd();
        
        // Place new bytes at file end to make room for shifting over all bytes.
        moveCursor(end+1);
        fileBuffer->sputn(bytes, n);

        // Shift over all bytes in front of cursor to make room for new byte.
        for (unsigned long long i = end+1; i > cursor; i--) {
            moveCursor(i-n);
            byte current = getCurrentByte();
            moveCursor(i);
            fileBuffer->sputc(current);
        }

        // Move cursor back and finnally insert byte.
        moveCursor(cursor);
        fileBuffer->sputn(bytes, n);

        // Return reference to self.
        return *this;
    }

    file& file::replaceByte(const byte in)
    {
        incCursor();
        fileBuffer->sputc(in);
        decCursor();
        return (*this);
    }

    file& file::replaceBytes(const byte* in, const int n)
    {
        fileBuffer->sputn(in, n);
        return (*this);
    }
}
