#include "hdetect.h"

bool compareHeaders(std::vector<int> header, int* buffer)
{
    for (long unsigned int i = 0; i < header.size(); i++)
        if (header[i] != buffer[i] && header[i] != -1)
            return false;
    return true;
}

std::map<std::string, std::vector<int>> headers;

namespace utils
{
    std::string getFileHeaderType(utils::file &f)
    {
        // Get current cursor position.
        auto cursor = f.getCursorLocation();

        // File headers and assosicated bytes. (-1 is used a wildcard).
        headers["AVI (Audio Video Interleave video format)"] = {0x52, 0x49, 0x46, 0x46, -1, -1, -1, -1, 0x41, 0x56, 0x49, 0x20};
        headers["BPG (Better Portable Graphics format)"] = {0x42, 0x50, 0x47, 0xFB};
        headers["BMP (Bitmap format)"] = {0x42, 0x4D};
        headers["CWK (AppleWorks 5 document)"] = {0x05, 0x07, 0x00, 0x00, 0x42, 0x4F, 0x42, 0x4F, 0x05, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};
        headers["CWK (AppleWorks 6 document)"] = {0x06, 0x07, 0xE1, 0x00, 0x42, 0x4F, 0x42, 0x4F, 0x06, 0x07, 0xE1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};
        headers["CLASS (Java class file)"] = {0xCA, 0xFE, 0xBA, 0xBE};
        headers["DEB (Linux deb file)"] = {0x21, 0x3C, 0x61, 0x72, 0x63, 0x68, 0x3E};
        headers["DMG (Apple Disk Image)"] = {0x78, 0x01, 0x73, 0x0D, 0x62, 0x62, 0x60};
        headers["FLAC (Free Lossless Audio Codec)"] = {0x66, 0x4C, 0x61, 0x43};
        headers["GIF87a (Graphics Interchange Format)"] = {0x47, 0x49, 0x46, 0x38, 0x37, 0x61};
        headers["GIF89a (Graphics Interchange Format)"] = {0x47, 0x49, 0x46, 0x38, 0x39, 0x61};
        headers["ICON (Computer Icon format)"] = {0,1,0,0};
        headers["ISO (ISO9660 CD/DVD image file)"] = {0x43, 0x44, 0x30, 0x30, 0x31};
        headers["JPEG (raw format)"] = {0xFF, 0xD8, 0xFF, 0xDB};
        headers["JPEG (raw format) "] = {0xFF, 0xD8, 0xFF, 0xEE};
        headers["JPEG (JFIF format)"] = {0xFF, 0xD8, 0xFF, 0xE0, 0x00, 0x10, 0x4A, 0x46, 0x49, 0x46, 0x00, 0x01};
        headers["JPEG (Exif format)"] = {0xFF, 0xD8, 0xFF, 0xE1, -1, -1, 0x45, 0x78, 0x69, 0x66, 0x00, 0x00};
        headers["LZIP (lzip compressed file)"] = {0x4C, 0x5A, 0x49, 0x50};
        headers["macOS file Alias (Symbolic link)"] = {0x62, 0x6F, 0x6F, 0x6B, 0x00, 0x00, 0x00, 0x00, 0x6D, 0x61, 0x72, 0x6B, 0x00, 0x00, 0x00, 0x00};
        headers["MP3 (MPEG-1 Layer 3)"] = {0xFF, 0xFB};
        headers["MP3 (MPEG-1 Layer 3 with an ID3v2 container)"] = {0x49, 0x44, 0x33};
        headers["DOS MZ executable "] = {0x4D, 0x5A};
        headers["PDF (pdf document)"] = {37, 80, 68, 70, 45};
        headers["PSD (Adobe Photoshop Document file)"] = {0x38, 0x42, 0x50, 0x53};
        headers["PNG (Portable Network Graphics)"] = {137, 80, 78, 71, 13, 10, 26, 10};
        headers["PCAP (libpcap format)"] = {0xa1, 0xb2, 0xc3, 0xd4};
        headers["PCAP (libpcap format) "] = {0xd4, 0xc3, 0xb2, 0xa1};
        headers["RAR (rar archive version 1.5 - 5.0)"] = {0x52, 0x61, 0x72, 0x21, 0x1A, 0x07, 0x00};
        headers["RAR (rar archive version 5.0+)"] = {0x52, 0x61, 0x72, 0x21, 0x1A, 0x07, 0x01, 0x00};
        headers["SQLITE (SQLite format 3)"] = {0x53, 0x51, 0x4c, 0x69, 0x74, 0x65, 0x20, 0x66, 0x6f, 0x72, 0x6d, 0x61, 0x74, 0x20, 0x33, 0x00};
        headers["TAR (tar zip using Lempel-Ziv-Welch compression)"] = {0x1F, 0x9D};
        headers["TAR (tar zip using LZH compression)"] = {0x1F, 0xA0};
        headers["TDF (Telegram Desktop File)"] = {0x54, 0x44, 0x46, 0x24};
        headers["TIFF (Tagged Image File Format, little endian format)"] = {0x49, 0x49, 0x2A, 0x00};
        headers["TIFF (Tagged Image File Format, bif endian format)"] = {0x4D, 0x4D, 0x00, 0x2A};
        headers["TDEF (Telegram Desktop Encrypted File)"] = {0x54, 0x44, 0x45, 0x46};
        headers["WASM (WebAssembly binary format)"] = {0x00, 0x61, 0x73, 0x6d};
        headers["WAV (Waveform Audio File format)"] = {0x52, 0x49, 0x46, 0x46, -1, -1, -1, -1, 0x57, 0x41, 0x56, 0x45};
        headers["Windows Update Binary (Delta Compression)"] = {0x44, 0x43, 0x4D, 0x01, 0x50, 0x41, 0x33, 0x30};
        headers["XML (eXtensible Markup Language)"] = {0x3c, 0x3f, 0x78, 0x6d, 0x6c, 0x20};
        
        // Move cursor to file start.
        f.moveCursor(0);

        // Load first 32 bytes of file into buffer.
        int buffer[32];
        for (int i = 0; i < 32; i++) {
            buffer[i] = f.getCurrentByte();
            f.incCursor();
        }

        // Reset cursor.
        f.moveCursor(cursor);

        // Compare headers with simple linear search.
        for (auto const &item : headers)
            if (compareHeaders(item.second,buffer))
                return item.first;

        return std::string("");
    }

    unsigned long long getHeaderLength(const std::string& header)
    {
        return headers[header].size();
    }
}
