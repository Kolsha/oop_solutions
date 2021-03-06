#pragma once

#include <iostream>
#include <vector>
#include <stack>
#include <memory>


namespace{

std::stack<std::string> clipboard;

}

// Receiver
class Document: public std::string
{
public:
    Document() {}
    Document(const std::string& txt){
        this->assign(txt);
    }
    Document(std::istream& in){
        std::string text(std::istreambuf_iterator<char>(in), {});
        this->assign(text);
    }
    virtual ~Document(){}
};


//Cmd
class Command
{
protected:
    bool executed = false;
    std::shared_ptr<Document> doc;
public:
    virtual void Execute() = 0;
    virtual void unExecute() = 0;
    virtual ~Command(){}
};



class UnRedoCmd: public Command{
private:
    bool redo = false;
public:
    UnRedoCmd(bool _redo = false) : redo(_redo) {}
    void Execute(){}
    void unExecute(){}
    bool is_redo(){
        return redo;
    }
    bool is_undo(){
        return !redo;
    }
};

// Invoker
class LineEditor
{
private:
    std::vector<std::shared_ptr<Command>> commands;
    size_t cur = 0;
public:
    LineEditor() {}
    void add(std::shared_ptr<Command> cmd)
    {
        if(UnRedoCmd* unre = dynamic_cast<UnRedoCmd*>(cmd.get()))
        {
            if(unre->is_redo()){
                redo();
            }else{
                undo();
            }

            return ;
        }

        size_t cmds_sz = commands.size();
        if(cmds_sz > 0 &&
                cur < (cmds_sz - 1)){
            commands.erase((commands.begin() + cur), commands.end());
        }
        commands.push_back(cmd);
        cmd->Execute();
        cur++;

    }

    void undo(size_t count = 1)
    {
        if(count < 1){
            count = 1;
        }

        for (size_t i = 0; i < count; i++){
            if (cur > 0)
                commands[--cur]->unExecute();
        }
    }

    void redo(size_t count = 1)
    {
        if(commands.size() < 1){
            return ;
        }
        if(count < 1){
            count = 1;
        }

        for (size_t i = 0; i < count; i++){
            if (cur <= (commands.size() - 1))
                commands[cur++]->Execute();
        }
    }
    virtual ~LineEditor(){}

};



class CopyCmd : public Command
{

    size_t start, end;
    size_t len(){
        if(start >= end || end < 1){
            return 0;
        }
        return (end - start);
    }

public:
    CopyCmd(std::shared_ptr<Document> _doc,
            size_t idx1 = 0, size_t idx2 = 0):
        start(idx1), end(idx2)
    {
        this->doc = _doc;
    }

    void Execute(){

        size_t ln = len();
        if(ln > 0 && start < doc->length()){
            std::string cp =  doc->substr(start, ln);
            clipboard.push(std::move(cp));
            executed = true;
        }
    }

    void unExecute(){
        if(len() > 0 && executed){
            clipboard.pop();
            executed = false;
        }
    }
    virtual ~CopyCmd(){}
};


class PasteCmd : public Command
{
    size_t idx, original_size = 0;
    std::string str_pasted;

public:
    PasteCmd(std::shared_ptr<Document> _doc,
             size_t _idx):
        idx(_idx)
    {
        this->doc = _doc;
    }

    void Execute(){
        if(clipboard.size() > 0){
            str_pasted = clipboard.top();
            if(doc->size() <= idx){
                original_size = doc->length();
                doc->resize((idx), '\n');
            }
            doc->insert(idx, str_pasted);
            clipboard.pop();
            executed = true;
        }
    }

    void unExecute(){
        if(executed){
            doc->erase(idx, str_pasted.length());
            clipboard.push(str_pasted);
            if(original_size > 0)
                doc->resize(original_size);
            executed = false;

        }
    }
    virtual ~PasteCmd(){}
};



class InsertCmd : public Command
{
    std::string str;
    size_t idx, original_size = 0;
public:
    InsertCmd(std::shared_ptr<Document> _doc,
              std::string _str = "", size_t _idx = 0):
        str(_str), idx(_idx)
    {
        this->doc = _doc;
    }

    void Execute(){
        if(str.length() < 1){
            return ;
        }
        if(doc->size() <= idx){
            original_size = doc->length();
            doc->resize((idx), '\n');
        }
        doc->insert(idx, str);
        executed = true;
    }

    void unExecute(){
        if(str.length() < 1){
            return ;
        }
        doc->erase(idx, str.length());
        if(original_size > 0)
            doc->resize(original_size);

        executed = false;
    }
    virtual ~InsertCmd(){}
};





class DelCmd : public Command
{
    size_t start, end;
    size_t len(){
        if(start >= end || end < 1){
            return 0;
        }
        return (end - start);
    }

    std::string deleted = "";


public:
    DelCmd(std::shared_ptr<Document> _doc,
           size_t idx1 = 0, size_t idx2 = 0):
        start(idx1), end(idx2)
    {
        this->doc = _doc;
    }

    void Execute(){
        size_t ln = len();
        if(ln > 0 && start < doc->length()){
            deleted =  doc->substr(start, ln);
            doc->erase(start, ln);
            executed = true;
        }
    }

    void unExecute(){
        if(len() > 0 && deleted.length() > 0){
            doc->insert(start, deleted);
            executed = false;
        }
    }
    virtual ~DelCmd(){}
};





