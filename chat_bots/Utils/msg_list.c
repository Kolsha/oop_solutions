#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "msg_list.h"

char *build_request(char *frmt, ...){

    va_list args;

    va_start(args, frmt);

    size_t needed = vsnprintf(NULL, 0, frmt, args) + 1;
    char  *buffer = malloc(needed);
    if(buffer == NULL){
        return NULL;
    }
    int ret;

    ret = vsprintf(buffer, frmt, args);
    va_end(args);

    if(ret < 0){
        free(buffer);
        return NULL;
    }

    return buffer;
}

size_t my_strlen(char *s){

    return (s == NULL) ? 0 : strlen(s);
}

char *my_strstr(char *haystack,
                char *needle){ //horspul

    unsigned char *str = (unsigned char*) haystack;
    unsigned char *substr = (unsigned char*) needle;
    int lenstr = my_strlen(str);
    int lensubstr = my_strlen(substr);

    if (lenstr < 1 || lensubstr < 1 || lensubstr > lenstr){
        return NULL;
    }

    char *table = malloc(256 * sizeof(char));
    if(table == NULL){
        return NULL;
    }

    memset(table, lensubstr, 256 * sizeof(char));
    for(int c = lensubstr - 2; c > 0; c--){
        if(table[(int)substr[c]] == lensubstr)
            table[(int)substr[c]] = lensubstr - c;
    }

    for(int i = 0; i <= (lenstr - lensubstr); i++){
        if (str[i] == substr[0]){
            int match = 1;
            for(int c = 0; c < lensubstr; c++)
            {
                match = (str[i + c] == substr[c]);
                if(!match){
                    break;
                }
            }

            if(match)
            {
                free(table);
                return &(str[i]);
            }
            else{
                int stop = (int)str[i + lensubstr - 1];
                i += table[stop];
            }

        }
    }
    free(table);
    return NULL;

}

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

MsgList *find_msg(MsgList *head, char* text){
    if(head == NULL || text == NULL){
        return NULL;
    }

    MsgList *runner = head;
    while(runner != NULL){
        if(my_strstr(runner->text, text) != NULL){
            return runner;
        }
        runner = runner->next;
    }
    return NULL;
}


void free_msgs(MsgList *head){

    if(head == NULL){
        return ;
    }

    MsgList *runner = (MsgList*)head, *tmp;
    while(runner != NULL){
        free(runner->text);
        free(runner->username);
        tmp = runner->next;
        free(runner);
        runner = tmp;
    }
}
