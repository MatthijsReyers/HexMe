#include "file.hpp"
#include "file-headers.hpp"

#include <unistd.h>
#include <ncurses.h>
#include <sstream>
#include <iostream>

File::File() {}

File::File(const char* p) : path(p)
{
    this->load(this->path);
}

File::File(const std::string& p) : path(p)
{
    this->load(p);
}

File& File::load(const char* path)
{
    return this->load(std::string(path));
}

File& File::load(const std::string& path)
{
    this->path = path;
    this->unsavedChanges = false;

    std::ifstream instream(path, std::ios::in | std::ios::binary);
    this->data = std::vector<uint8_t>((std::istreambuf_iterator<char>(instream)), std::istreambuf_iterator<char>());
    
    // TODO: throw FailedToOpenFileException("Failed to open HexFile", path);

    this->identifyFileType();


    return *this;
}

File& File::save()
{
    // Even if the app does not know about any unsaved changes, the file may have been modified by
    // an external program so we'll just always write it to disk.
    return this->save(this->path);
}

File& File::save(const char* path)
{
    return this->save(std::string(path));
}

File& File::save(const std::string& path)
{
    std::ofstream file(path, std::ios::trunc);
    file.write((const char *)&this->data[0], this->data.size());
    file.close();
    this->path = path;
    this->unsavedChanges = false;
    return *this;
}

bool compareHeaders(std::vector<u_int8_t>& h1, std::vector<int16_t>& h2)
{
    size_t min_size = std::min(h1.size(), h2.size());
    if (min_size < 2) return false;
    for (size_t i = 0; i < min_size; i++)
    {
        if (h1[i] != h2[i]) return false;
    }
    return true;
}

void File::identifyFileType() 
{
    for (auto header : fileHeaders) {
        if (compareHeaders(this->data, header.second)) {
            this->fileType = header;
            return;
        }
    }
    this->fileType.reset();
}

std::string File::getPath()
{
    return this->path;
}

std::string File::getName()
{
    std::size_t found = path.find_last_of("/\\");
    if (found == std::string::npos) {
        return path;
    }
    return path.substr(found + 1, path.size());
}

size_t File::size()
{
    return this->data.size();
}

size_t File::getCursor()
{
    return std::min(this->data.size() - 1, this->cursor);
}

void File::resetCursor()
{
    this->cursor = 0;
}

void File::setCursor(size_t location)
{
    this->cursor = std::min(this->data.size() - 1, location);
}

bool File::incCursor()
{
    return this->incCursor(1);
}

bool File::incCursor(const size_t n)
{
    auto tmp = this->cursor+n;
    if (tmp >= this->data.size()) {
        return false;
    }
    this->setCursor(tmp);
    return true;
}

bool File::decCursor()
{
    if (this->cursor > 0) {
        setCursor(this->cursor - 1);
        return true;
    }
    return false;
}

bool File::decCursor(const size_t n)
{
    if (n > this->cursor) {
        return false;
    }
    this->setCursor(this->cursor - std::min(this->cursor, n));
    return true;
}

size_t File::getBytesAfterCursor()
{
    return this->data.size() - this->getCursor();
}

u_int8_t File::getCurrentByte()
{
    // Should never happen but check anyway just in case..
    if (this->cursor > this->data.size()) {
        throw std::out_of_range("Cursor is beyond the data size");
    }
    return this->data[this->cursor];
}

std::optional<u_int8_t> File::getByte(const size_t address)
{
    if (address < this->data.size()) {
        return this->data[address];
    }
    return std::optional<u_int8_t>();
}

void File::insertByte(const u_int8_t b)
{
    this->unsavedChanges = true;
    this->data.insert(this->data.begin() + this->cursor, b);

    // Could the user have overwritten the file header/signature?
    if (this->cursor < MAX_FILE_HEADER_LEN) {
        this->identifyFileType();
    }
}

void File::insertBytes(const std::vector<u_int8_t>& bs)
{
    this->unsavedChanges = true;
    this->data.insert(this->data.begin() + this->cursor, bs.begin(), bs.end());

    // Could the user have overwritten the file header/signature?
    if (this->cursor < MAX_FILE_HEADER_LEN) {
        this->identifyFileType();
    }
}

void File::overwriteByte(const u_int8_t in)
{
    this->overwriteBytes({ in });
}

void File::overwriteBytes(const std::vector<u_int8_t>& bs)
{
    // Should never happen but check anyway just in case..
    if (this->cursor > this->data.size()) {
        throw std::out_of_range("Cursor is beyond the data size");
    }

    this->unsavedChanges = true;
    
    size_t copyEnd = this->cursor + bs.size();
    
    // Do the new bytes fit inside the bounds of the file data without extending?
    if (this->data.size() > copyEnd) {
        std::copy(bs.begin(), bs.end(), this->data.begin() + this->cursor);
    } 
    else {
        // New data only partially fits, overwrite what we can and append the rest at the end.
        size_t overlap = this->data.size() - this->cursor;
        std::copy(bs.begin(), bs.begin() + overlap, this->data.begin() + this->cursor);
        this->data.insert(this->data.end(), bs.begin() + overlap, bs.end());
    }

    // Could the user have overwritten the file header/signature?
    if (this->cursor < MAX_FILE_HEADER_LEN) {
        this->identifyFileType();
    }
}

void File::deleteByte() {
    this->deleteBytes(1);
}

void File::deleteBytes(const unsigned int n) {
    this->unsavedChanges = true;
    size_t cursor = getCursor();
    this->data.erase(
        this->data.begin() + cursor, 
        std::min(this->data.begin() + cursor + n, this->data.end())
    );
    // Could the user have overwritten the file header/signature?
    if (this->cursor < MAX_FILE_HEADER_LEN) {
        this->identifyFileType();
    }
}
