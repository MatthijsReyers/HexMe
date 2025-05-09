#pragma once

#include <optional>
#include <string>
#include <fstream>
#include <vector>

using FileHeader = std::pair<std::string, std::vector<int16_t>>;

/**
 * Hex editor file, this is essentially just an abstraction around a C++ file stream that loads
 * the contents of the file into memory to make it easier to manipulate and to temporarily 
 */
class File
{
private:
    /**
     * The actual data in the file + any changes the user may have made.
     */
    std::vector<u_int8_t> data = {};

    /**
     * Location of the file
     */
    std::string path = "";

    /**
     * Current cursor position as index of byte within the file.
     */
    size_t cursor = 0;

    /**
     * Looks at the file data to see if it contains a known file header and updates the 
     */
    void identifyFileType();

public:
    /**
     * If a known file header is found, the type of file that is opened.
     */
    std::optional<FileHeader> fileType;

    /**
     * Does this file contain unsaved changes that should be written to disk or discarded before
     * closing the application?
     */
    bool unsavedChanges = false;

    File(); 
    File(const char* path); 
    File(const std::string& path);
    
    File(const File&) = delete;

    File& load(const char* path);
    File& load(const std::string& path);

    File& save();
    File& save(const char* path);
    File& save(const std::string& path);

    std::string getPath();
    std::string getName();

    size_t size();
    size_t getBytesAfterCursor();

    size_t getCursor();
    void setCursor(size_t location);
    void resetCursor();

    bool incCursor();
    bool incCursor(const size_t n);
    bool decCursor();
    bool decCursor(const size_t n);

    std::optional<u_int8_t> getByte(const size_t address);

    u_int8_t getCurrentByte();
    u_int8_t* getCurrentBytesN(const int n);

    void insertByte(const u_int8_t in);
    void insertBytes(const std::vector<u_int8_t>& bs);
    
    void overwriteByte(const u_int8_t in);
    void overwriteBytes(const std::vector<u_int8_t>& bs);

    void deleteByte();
    void deleteBytes(const unsigned int n);
};

struct FailedToOpenFileException
{
    public:
        std::string message, path;
        FailedToOpenFileException(const std::string msg, const std::string file) : message(msg), path(file) {}
};
