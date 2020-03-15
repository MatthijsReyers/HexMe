#include "./file.h"

namespace utils
{
    file& file::open(const char* url)
    {
        return this->open(std::string(url));
    }

    file& file::open(const std::string url)
    {
        fileStream.open(url);
        if (!fileStream.is_open())
            throw FailedToOpenFileException(url, "does the file exist?");
        return (*this);
    }

    file& file::moveCursor(unsigned long long location)
    {
        cursor = location;
        return (*this);
    }

    unsigned long long file::getNextLocation(byte* bytes, int length)
    {
        unsigned long long temp = this->getCursorLocation();
        
    }

    file& file::insertBytes(const byte* bytes, const int length)
    {
        byte temp ;
        unsigned long long endingIndex = fileStream.end - cursor + length;
        for (unsigned long long i = cursor; i < endingIndex + length; i++)
        {
            // Get next byte.
            fileStream.seekp(static_cast<std::streampos>(cursor - 1));
            
            fileStream.seekp(static_cast<std::streampos>(cursor - 1));
        }
        

        // Create buffer to store modified bytes.
        byte* buffer = new byte[length];

        // Load new bytes into buffer.
        for (int i = 0; i < length; i++)
            buffer[i] = bytes[i];

        // Fill buffer with rest of the bytes after the cursor.
        fileStream.seekg(cursor);
        fileStream.read(buffer + length, cursor);

        // Overwrite bytes after cursor with new bytes.
        fileStream.seekp(static_cast<std::streampos>(cursor - 1));
        int bufferLength = 9;
        for (int i = 0; i < bufferLength; i++)
            fileStream.put(buffer[i]);
        
        // Reset state.
        fileStream.seekg(cursor);
        delete[] buffer;
        return (*this);
    }
}