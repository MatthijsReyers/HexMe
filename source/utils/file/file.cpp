#include "file.h"

#include "./../../utils/hdetect/hdetect.h"

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

    std::string file::getFileName()
    {
        std::size_t found = url.find_last_of("/\\");
        return url.substr(0,found);
    }

    std::string file::getHeader()
    {
        return header;
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

    // byte* file::getCurrentBytesN(const int n)
    // {
    //     return new byte[8]{0x03, 0x02, 0x0F, 0x06, 0x0A, 0x04, 0x02, 0x10};

    //     byte* result = new byte[n];
    //     fileBuffer->sgetn(result,n);
    //     return result;
    // }

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
