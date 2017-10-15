#include <iostream>

#include "line_editor.h"

int main()
{
    using namespace std;
    std::shared_ptr<Document> doc(new Document("teststst"));
    LineEditor editor;

    std::shared_ptr<UndoCmd> undo(new UndoCmd());
    editor.add(undo);

    *doc += "12344555";

    CopyCmd cp(doc, 0, 9999);
    cp.Execute();

    PasteCmd ps(doc, 0);
    ps.Execute();

    /* InsertCmd inst(doc, "_KOLSHA_", 100);
    inst.Execute();

    hacktoberfest
*/



    cout << *doc << endl;

    DelCmd del(doc, 0, 10);
    del.Execute();
    cout << *doc << endl;
    del.unExecute();
    cout << *doc << endl;


}

