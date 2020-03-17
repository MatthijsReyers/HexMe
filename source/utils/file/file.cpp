#include "./file.h"

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
        return (*this);
    }

    file& file::close()
    {
        this->fileStream->close();
        this->fileBuffer->close();
        return (*this);
    }

    std::string file::getFileName()
    {
        std::size_t found = url.find_last_of("/\\");
        return url.substr(0,found);
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

    byte file::getNextByte()
    {
        // return fileBuffer->sgetc();
    }

    byte* file::getCurrentBytesN(const int n)
    {
        return new byte[8]{0x03, 0x02, 0x0F, 0x06, 0x0A, 0x04, 0x02, 0x10};

        byte* result = new byte[n];
        fileBuffer->sgetn(result,n);
        return result;
    }

    byte* file::getNextBytesN(const int n)
    {

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
