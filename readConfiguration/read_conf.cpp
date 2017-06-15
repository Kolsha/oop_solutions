#include <iostream>
#include <string>
#include <map>

#include "read_conf.h"

using std::string;


namespace {

void insert_error(string *perrors,const  string & err){
    if(perrors != nullptr){
        perrors->append(err);
        perrors->append("\n");
    }
}

bool is_escaped(const string & line, string::size_type pos){

    size_t count_bs = 0;
    while(line[pos] == '\\'){
        count_bs++;

        if(pos == 0){
            break;
        }
        pos--;
    }

    return (count_bs % 2) != 0;
}

bool is_quote(char c){
    return (c == '"' || c == '\'');
}

string parse_name(const string & line, string::size_type &pos){

    string::size_type nameBegin = line.find_first_not_of(WHITE_SPACES, pos);
    if (nameBegin == string::npos)
    {
        return "";
    }

    bool quoted = false;
    string find_next = WHITE_SPACES;

    if(line[nameBegin] == '#'){
        return "";
    }

    if(is_quote(line[nameBegin])){
        find_next.clear();
        find_next.push_back(line[nameBegin]);
        nameBegin++;
        quoted = true;
    }

    string::size_type tmp_pos = nameBegin;
    string::size_type nameEnd = nameBegin;
    for(;;){
        nameEnd =
                line.find_first_of(find_next, tmp_pos);

        if (nameEnd == string::npos)
        {
            pos = nameEnd;
            return line.substr(nameBegin);
        }

        if(nameBegin == nameEnd){
            return "";
        }

        if(!is_escaped(line, nameEnd - 1)){
            break;
        }else{
            tmp_pos++;
        }
    }

    tmp_pos = nameBegin;
    while(tmp_pos < nameEnd){
        tmp_pos = line.find_first_of(COMMENT_STR, tmp_pos);
        if (tmp_pos == string::npos ||
                tmp_pos > nameEnd)
        {
            break;
        }
        bool escaped = is_escaped(line, tmp_pos - 1);
        if(!escaped && quoted){
            return "";
        }
        else if(escaped && !quoted){
            return line.substr(nameBegin , (tmp_pos - nameBegin));
        }
        tmp_pos++;
    }



    pos = nameEnd;
    if(quoted){
        pos++;
    }

    return line.substr(nameBegin , (nameEnd - nameBegin));

}

}

std::map<string, string> readConfiguration(std::istream &is,
                                      std::string *perrors){

    std::map<string, string> res;
    string line;
    size_t line_num = 0;
    while(getline(is, line)){
        line_num++;
        try{
            string::size_type pos = 0;
            string key = parse_name(line, pos);

            if(!key.length()){
                insert_error(perrors,
                             "Bad key on line "
                             + std::to_string(line_num));
                continue;
            }

            if (line[pos] != ' ' &&
                    line[pos] != '\t')
            {

                insert_error(perrors,
                             "Comment in key on line "
                             + std::to_string(line_num));

                continue;
            }

            string value = parse_name(line, pos);


            string::size_type tmp_pos =
                    line.find_first_not_of(WHITE_SPACES, pos);
            if (tmp_pos != string::npos)
            {

                if(line[tmp_pos] != '#' &&
                        line[tmp_pos] != '\0'){

                    insert_error(perrors,
                                 "Bad value on line "
                                 + std::to_string(line_num));

                    continue;
                }

            }

            res.insert(std::pair<string,string>(key, value));
        }
        catch(std::exception e){
            insert_error(perrors, e.what());
        }
    }

    return res;

}

