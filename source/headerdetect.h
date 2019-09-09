#include <iostream>
#include <vector>
#include <array>
#include <map>

using string = std::string;

bool checkForHeader(int* buffer, std::vector<int> header, int headerLength)
{
    bool foundHeader = true;
    for (int i = 0; i < headerLength; i++) {if (buffer[i] != header[i] && header[i] != -1) foundHeader = false;};
    return foundHeader;
}

string getFileHeader(int* buffer)
{
    std::map<string, std::vector<int>> headers;
    
    // File headers and assosicated bytes.
    headers["PDF (pdf document)"] = {37, 80, 68, 70, 45};
    headers["PSD (Adobe Photoshop Document file)"] = {0x38, 0x42, 0x50, 0x53};
    headers["PNG (Portable Network Graphics)"] = {137, 80, 78, 71, 13, 10, 26, 10};
    headers["JPEG (raw format)"] = {0xFF, 0xD8, 0xFF, 0xDB};
    headers["JPEG (raw format) "] = {0xFF, 0xD8, 0xFF, 0xEE};
    headers["JPEG (JFIF format)"] = {0xFF, 0xD8, 0xFF, 0xE0, 0x00, 0x10, 0x4A, 0x46, 0x49, 0x46, 0x00, 0x01};
    headers["JPEG (Exif format)"] = {0xFF, 0xD8, 0xFF, 0xE1, -1, -1, 0x45, 0x78, 0x69, 0x66, 0x00, 0x00};
    headers["PCAP libpcap format"] = {0xa1, 0xb2, 0xc3, 0xd4};
    headers["PCAP libpcap format "] = {0xd4, 0xc3, 0xb2, 0xa1};
    headers["ICON (Computer Icon format)"] = {0,1,0,0};
    headers["RAR archive (version 1.5 - 5.0)"] = {0x52, 0x61, 0x72, 0x21, 0x1A, 0x07, 0x00};
    headers["RAR archive (version 5.0+)"] = {0x52, 0x61, 0x72, 0x21, 0x1A, 0x07, 0x01, 0x00};
    headers["WAV (Waveform Audio File Format)"] = {0x52, 0x49, 0x46, 0x46, -1, -1, -1, -1, 0x57, 0x41, 0x56, 0x45};

    // Linear search all headers.
    for (auto item : headers)
    {
        if (checkForHeader(buffer, item.second, item.second.size()))
        {
            return item.first;
        }
    }
    
    // Return nothing if nothing found.
    return "";
}
