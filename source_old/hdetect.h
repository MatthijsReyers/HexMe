#include <iostream>
#include <fstream>
#include <vector>
#include <map>

bool checkForHeader(int* buffer, std::vector<int> header, int headerLength)
{
    bool foundHeader = true;
    for (int i = 0; i < headerLength; i++) {if (buffer[i] != header[i] && header[i] != -1) foundHeader = false;};
    return foundHeader;
}

void getFileHeader(std::fstream &file, std::string &fileHeader, int &fileHeaderLen)
{
    std::map<std::string, std::vector<int>> headers;
    
    // File headers and assosicated bytes.
    headers["PDF (pdf document)"] = {37, 80, 68, 70, 45};
    headers["PSD (Adobe Photoshop Document file)"] = {0x38, 0x42, 0x50, 0x53};
    headers["PNG (Portable Network Graphics)"] = {137, 80, 78, 71, 13, 10, 26, 10};
    headers["JPEG (raw format)"] = {0xFF, 0xD8, 0xFF, 0xDB};
    headers["JPEG (raw format) "] = {0xFF, 0xD8, 0xFF, 0xEE};
    headers["JPEG (JFIF format)"] = {0xFF, 0xD8, 0xFF, 0xE0, 0x00, 0x10, 0x4A, 0x46, 0x49, 0x46, 0x00, 0x01};
    headers["JPEG (Exif format)"] = {0xFF, 0xD8, 0xFF, 0xE1, -1, -1, 0x45, 0x78, 0x69, 0x66, 0x00, 0x00};
    headers["GIF87a (Graphics Interchange Format)"] = {0x47, 0x49, 0x46, 0x38, 0x37, 0x61};
    headers["GIF89a (Graphics Interchange Format)"] = {0x47, 0x49, 0x46, 0x38, 0x39, 0x61};
    headers["PCAP (libpcap format)"] = {0xa1, 0xb2, 0xc3, 0xd4};
    headers["PCAP (libpcap format) "] = {0xd4, 0xc3, 0xb2, 0xa1};
    headers["ICON (Computer Icon format)"] = {0,1,0,0};
    headers["RAR (rar archive version 1.5 - 5.0)"] = {0x52, 0x61, 0x72, 0x21, 0x1A, 0x07, 0x00};
    headers["RAR (rar archive version 5.0+)"] = {0x52, 0x61, 0x72, 0x21, 0x1A, 0x07, 0x01, 0x00};
    headers["WAV (Waveform Audio File format)"] = {0x52, 0x49, 0x46, 0x46, -1, -1, -1, -1, 0x57, 0x41, 0x56, 0x45};
    headers["AVI (Audio Video Interleave video format)"] = {0x52, 0x49, 0x46, 0x46, -1, -1, -1, -1, 0x41, 0x56, 0x49, 0x20};
    headers["ISO (ISO9660 CD/DVD image file)"] = {0x43, 0x44, 0x30, 0x30, 0x31};
    headers["MP3 (MPEG-1 Layer 3)"] = {0xFF, 0xFB};
    headers["MP3 (MPEG-1 Layer 3 with an ID3v2 container)"] = {0x49, 0x44, 0x33};
    headers["XML (eXtensible Markup Language)"] = {0x3c, 0x3f, 0x78, 0x6d, 0x6c, 0x20};
    headers["macOS file Alias (Symbolic link)"] = {0x62, 0x6F, 0x6F, 0x6B, 0x00, 0x00, 0x00, 0x00, 0x6D, 0x61, 0x72, 0x6B, 0x00, 0x00, 0x00, 0x00};

    // Fill buffer with first 32 bytes.
    int buffer[32];
    for (int i = 0; i < 32; i++) {buffer[i] = file.get();}

    // Linear search all headers.
    for (auto const &item : headers)
    {
        if (checkForHeader(buffer, item.second, item.second.size()))
        {
            fileHeaderLen = item.second.size();
            fileHeader = item.first;
            return;
        }
    }
    
    // Return nothing if nothing found.
    fileHeader = "";
    fileHeaderLen = 0;
}
