#pragma once

#include <iostream>
#include "line_editor.h"

class CmdParser
{
private:
    std::vector<std::shared_ptr<Command>> commands;
    std::shared_ptr<Command> parse_cmd(const std::string& cmd);
    std::shared_ptr<Document> doc;
public:
    CmdParser(std::istream& in, std::shared_ptr<Document> _doc);
    std::vector<std::shared_ptr<Command>>::iterator begin(){
        return commands.begin();
    }

    std::vector<std::shared_ptr<Command>>::iterator end(){
        return commands.end();
    }

    virtual ~CmdParser(){}
};
