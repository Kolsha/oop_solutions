#ifndef MSG_LIST_H
#define MSG_LIST_H
#define PRINT_MSG 1

#include <stdlib.h>




typedef void *Pointer;typedef struct tMsgList{

    char* text;
    char* username;
    struct tMsgList *next;

} MsgList;

MsgList *insert_msg(MsgList *head,char* username, char* text);

#endif
