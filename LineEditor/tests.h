#pragma once

#include <iostream>
#include <sstream>

#include "gtest/gtest.h"
#include "line_editor.h"
#include "cmd_parser.h"


using namespace std;


TEST(LineEditorTest, DelTest) {
    string str = "Billy always listens to his mother. He always does what she says.";
    shared_ptr<Document> doc(new Document(str));
    LineEditor editor;
    stringstream ss("delete 0, 6\n"
                    "delete 12, 19\n"
                    "delete 2, 7\n"
                    "delete 2, 7\n"
                    "delete 4, 16\n"
                    "delete 5, 99"
                    );
    CmdParser parser(ss, doc);

    for(auto &cmd: parser){
        editor.add(cmd);
    }

    ASSERT_EQ(*doc, "alisa");

    editor.undo(999);
    ASSERT_EQ(*doc, str);

}


TEST(LineEditorTest, InsertTest) {
    shared_ptr<Document> doc(new Document());
    LineEditor editor;
    stringstream ss("insert 0,\"!\"\n"
                    "insert 0,\"World\"\n"
                    "insert 0,\"My \"\n"
                    "insert 0,\"Hello \"\n");
    CmdParser parser(ss, doc);

    for(auto &cmd: parser){
        editor.add(cmd);
    }


    ASSERT_EQ(*doc, "Hello My World!");

    editor.undo();
    ASSERT_EQ(*doc, "My World!");



    editor.undo();
    ASSERT_EQ(*doc, "World!");

    editor.undo();
    ASSERT_EQ(*doc, "!");

    editor.redo(999);
    ASSERT_EQ(*doc, "Hello My World!");
}


TEST(LineEditorTest, CopyTest) {
    string str = "ABC";
    shared_ptr<Document> doc(new Document(str));
    ASSERT_TRUE(str == *doc);
    LineEditor editor;
    stringstream ss("copy 0, 3\n"
                    "paste 3\n" );
    CmdParser parser(ss, doc);

    for(auto &cmd: parser){
        editor.add(cmd);
    }
    ASSERT_TRUE(*doc == (str + str));
    editor.undo();
    ASSERT_TRUE(*doc == (str));
    editor.redo();
    ASSERT_TRUE(*doc == (str + str));
    editor.redo();
}



TEST(LineEditorTest, BadDataTest) {
    shared_ptr<Document> doc(new Document());
    LineEditor editor;
    stringstream ss("copy 0, 3\n"
                    "paste 3\n"
                    "delete 3, 4\n"
                    "\n"
                    "delete 33, 33\n"
                    "delete 0, 0\n"
                    "free 01, 33\n"
                    "delete undo, redo\n"
                    "redo\n"
                    "redo\n"
                    "redo\n"
                    "redo\n"
                    "redo\n"
                    "undo\n"
                    "undo\n"
                    "undo\n"
                    "undo\n"
                    "undo\n"
                    "");
    CmdParser parser(ss, doc);

    for(auto &cmd: parser){
        editor.add(cmd);
    }

    ASSERT_EQ(*doc, "");
}



