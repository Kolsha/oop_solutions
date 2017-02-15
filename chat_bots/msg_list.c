#include <stdlib.h>
#include <string.h>
#include "msg_list.h"


MsgList *insert_msg(MsgList *head, char *username, char *text){

    if(text == NULL || username == NULL){
        return NULL;
    }

    MsgList *res = (MsgList*)malloc(sizeof(MsgList));
    if(res == NULL){
        return NULL;
    }

    res->text = (char*)strdup(text);
    res->username = (char*)strdup(username);
    res->next = NULL;
#ifdef PRINT_MSG
    printf("%s: %s\n", username, text);
#endif

    MsgList *runner = head, *prev = NULL;
    while(runner != NULL){
        prev = runner;
        runner = runner->next;
    }
    if(prev != NULL){
        prev->next = res;
    }
    return res;
}

