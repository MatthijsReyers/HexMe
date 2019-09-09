#include <iostream>

using string = std::string;

bool checkForHeader(int* buffer, int* header, int headerLength)
{
    bool foundHeader = true;
    for (int i = 0; i < headerLength; i++) {if (buffer[i] != header[i] && header[i] != 999) foundHeader = false;};
    return foundHeader;
}

string getFileHeader(int* buffer)
{
    // Detect PDF header.
    int headerPDF[5] = {37, 80, 68, 70, 45};
    if (checkForHeader(buffer, headerPDF, 5)) return "PDF (pdf document)";

    // Detect PSD header.
    int headerPSD[4] = {0x38, 0x42, 0x50, 0x53};
    if (checkForHeader(buffer, headerPSD, 5)) return "PSD (Adobe Photoshop Document file)";

    // Detect PNG header.intint
    int headerPNG[8] = {137, 80, 78, 71, 13, 10, 26, 10};
    if (checkForHeader(buffer, headerPNG, 8)) return "PNG (Portable Network Graphics)";

    // Detect JPEG (raw) header.
    int headerJPEGraw[4] = {0xFF, 0xD8, 0xFF, 0xDB};
    if (checkForHeader(buffer, headerJPEGraw, 8)) return "JPEG (raw format)";
    int headerJPEGraw2[4] = {0xFF, 0xD8, 0xFF, 0xEE};
    if (checkForHeader(buffer, headerJPEGraw2, 8)) return "JPEG (raw format)";

    // Detect JPEG (JFIF) header.
    int headerJPEGjfif[12] = {0xFF, 0xD8, 0xFF, 0xE0, 0x00, 0x10, 0x4A, 0x46, 0x49, 0x46, 0x00, 0x01};
    if (checkForHeader(buffer, headerJPEGjfif, 8)) return "JPEG (JFIF format)";

    // Detect JPEG (EXIF) header.
    int headerJPEGexif[12] = {0xFF, 0xD8, 0xFF, 0xE1, 999, 999, 0x45, 0x78, 0x69, 0x66, 0x00, 0x00};
    if (checkForHeader(buffer, headerJPEGjfif, 8)) return "JPEG (Exif format)";

    // Detect PCAP header
    int headerPCAP[4] = {0xa1, 0xb2, 0xc3, 0xd4};
    if (checkForHeader(buffer, headerPCAP, 4)) return "PCAP libpcap format";
    int headerPCAP1[4] = {0xd4, 0xc3, 0xb2, 0xa1};
    if (checkForHeader(buffer, headerPCAP1, 4)) return "PCAP libpcap format";

    // ICO header
    int headerICON[4] = {0,1,0,0};
    if (checkForHeader(buffer, headerICON, 4)) return "ICON Computer icon";

    // RAR header 1.5
    int headerRAR[8] = {0x52, 0x61, 0x72, 0x21, 0x1A, 0x07, 0x00};
    if (checkForHeader(buffer, headerRAR, 8)) return "RAR archive (version 1.5 - 5.0)";

    // RAR header 5.0
    int headerRAR1[9] = {0x52, 0x61, 0x72, 0x21, 0x1A, 0x07, 0x01, 0x00};
    if (checkForHeader(buffer, headerRAR1, 8)) return "RAR archive (version 5.0+)";

    // Wave
    int headerWAV[12] = {0x52, 0x49, 0x46, 0x46, 999, 999, 999, 999, 0x57, 0x41, 0x56, 0x45};
    if (checkForHeader(buffer, headerWAV, 12)) return "WAV (Waveform Audio File Format)";

    // Return nothing if nothing found.
    return "";
}
