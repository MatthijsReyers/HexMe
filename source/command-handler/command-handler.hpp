#pragma once

#include "./../app/exceptions.h"
#include "./../file/file.hpp"

#include <string>
#include <map>
#include <memory>
#include <vector>

/**
 * This class is responsible for parsing and executing the commands entered by the user in the 
 * command text-box.
 */
class CommandHandler
{
private:
    typedef void (CommandHandler::*cmdHandler)(std::vector<std::string>& tokens);

    std::weak_ptr<File> file;
    std::map<std::string, cmdHandler> commands;

    std::vector<std::string> lexer(std::string cmd);

    void onEcho(std::vector<std::string>& tokens);
    void onExit(std::vector<std::string>& tokens);
    void onSave(std::vector<std::string>& tokens);
    void onHelp(std::vector<std::string>& tokens);
    void onOpen(std::vector<std::string>& tokens);
    void onGoto(std::vector<std::string>& tokens);
    void onMove(std::vector<std::string>& tokens);
    void onFind(std::vector<std::string>& tokens);
    void onInsert(std::vector<std::string>& tokens);
    void onReplace(std::vector<std::string>& tokens);
    void onDelete(std::vector<std::string>& tokens);

    /**
     * Parses an integer string like "-32" or "0x0FF" into an actual integer.
     */
    int64_t parseInt(std::string& val);

public:
    CommandHandler(std::weak_ptr<File>);

    void executeCmd(std::string& cmd);
};

struct CmdSyntaxErrorException : public HexMeException
{
    public:
        CmdSyntaxErrorException(std::string msg) : HexMeException(msg) {}
};
