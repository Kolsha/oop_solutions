#include <iostream>
#include <sstream>

#include "line_editor.h"
#include "cmd_parser.h"

int main()
{
    using namespace std;
    std::shared_ptr<Document> doc(new Document("ABCDEFG"));
    LineEditor editor;
    stringstream ss("copy 1, 3\n"
                    "insert 1,\"hello\"\n"
                    "paste 6\n"
                    "undo\n"
                    "redo\n"
                    "delete 2, 7\n"
                    "undo\n"
                    "undo\n"
                    "redo\n"
                    "redo"
                    );
    CmdParser parser(ss, doc);

    cout << "Before: " << *doc << endl;

    for(auto &cmd: parser){
        editor.add(cmd);
    }

    cout << " After: " << *doc << endl;



}

