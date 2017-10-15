#include <sstream>
#include <algorithm>
#include "cmd_parser.h"

namespace{

const std::string WHITE_SPACES = " \t";
const char COMMENT_STR = '#';

}

CmdParser::CmdParser(std::istream &in, std::shared_ptr<Document> _doc)
{
    doc = _doc;
    std::string line;
    size_t line_num = 0;
    while(std::getline(in, line)){
        line_num++;
        if(line.length() < 1 ||
                line[0] == COMMENT_STR){
            continue;
        }
        std::shared_ptr<Command> cmd = parse_cmd(line);
        if(cmd != nullptr){
            commands.push_back(cmd);
        }
    }

}


std::shared_ptr<Command> CmdParser::parse_cmd(const std::string &cmd)
{
    std::stringstream ss(cmd);
    std::string name;
    ss >> name;

    std::transform(name.begin(), name.end(), name.begin(), ::tolower);

    if(name == "undo"){
        return std::shared_ptr<Command>(new UndoCmd);
    }

    if(name == "redo"){
        return std::shared_ptr<Command>(new RedoCmd);
    }

    if(name == "copy" || name == "delete"){
        size_t idx1 = 0, idx2 = 0;
        ss >> idx1;
        ss.ignore(100, ',');
        ss >> idx2;
        idx1++;
        Command *cmd = nullptr;
        if(name == "copy"){
            cmd = new CopyCmd(doc, idx1, idx2);
        }else{
            cmd = new DelCmd(doc, idx1, idx2);
        }
        return std::shared_ptr<Command>(cmd);
    }

    if(name == "paste"){
        size_t idx = 0;
        ss >> idx;
        return std::shared_ptr<Command>(new PasteCmd(doc, idx));
    }

    if(name == "insert"){
        size_t idx = 0;
        std::string val = "";
        ss >> idx;
        ss.ignore(100, ',');
        ss >> val;
        auto it_begin = val.find_first_of('"');
        auto it_end = val.find_last_of('"');
        it_begin++;

        if(it_begin == std::string::npos ||
                it_end == std::string::npos){
            return nullptr;
        }

        val = val.substr(it_begin, (it_end - it_begin));
        if(val.empty()){
            return nullptr;
        }

        return std::shared_ptr<Command>(new InsertCmd(doc, val, idx));

    }

    return nullptr;


}
