#include "./file.h"

namespace utils
{
    file::file()
    {
        fileStream = new std::fstream();
    }

    file::file(const char* url)
    {
        fileStream = new std::fstream();
        this->open(std::string(url));
    }

    file::file(const std::string& url)
    {
        fileStream = new std::fstream();
        this->open(url);
    }

    file& file::open(const char* url)
    {
        return this->open(std::string(url));
    }

    file& file::open(const std::string& url)
    {
        fileStream->open(url, std::ios::out | std::ios::binary | std::ios::in);
        fileBuffer = fileStream->rdbuf();
        return (*this);
    }

    file& file::close()
    {
        this->fileStream->close();
        this->fileBuffer->close();
        return (*this);
    }

    file& file::resetCursor()
    {
        fileBuffer->pubseekpos(0);
        return (*this);
    }

    file& file::moveCursor(unsigned long long location)
    {
        fileBuffer->pubseekpos(0);
        return (*this);
    }

    file& file::incCursor()
    {
        //TODO
        return (*this);
    }

    byte file::getCurrentByte()
    {
        return fileBuffer->sgetc();
    }

    file& file::replaceByte(const byte in)
    {
        fileBuffer->sputc(in);
        return (*this);
    }

    file& file::replaceBytes(const byte* in, const int n)
    {
        fileBuffer->sputn(in, n);
        return (*this);
    }
}
