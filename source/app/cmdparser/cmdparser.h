#pragma once

#include "./../app.h"
#include "./../../utils/file/file.h"

#include <sstream>
#include <string>
#include <map>
#include <vector>

class cmdparser
{
private:
    typedef bool (cmdparser::*method)(std::vector<std::string>* tokens);

    std::string error;
    utils::file& file;
    app* hexme;
    std::map<std::string,method> commands;

    std::vector<std::string>* lexer(std::string cmd);

    bool onExit(std::vector<std::string>* tokens);
    bool onSave(std::vector<std::string>* tokens);
    bool onMove(std::vector<std::string>* tokens);

public:
    cmdparser(utils::file& file, app* hexme);

    bool executeCmd(std::string& cmd);
    std::string& getError();
};