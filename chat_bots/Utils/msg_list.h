#ifndef MSG_LIST_H
#define MSG_LIST_H
#define PRINT_MSG 1

char *build_request(char *frmt, ...);

typedef void *Pointer;typedef struct tMsgList{

    char* text;
    char* username;
    struct tMsgList *next;

} MsgList;

MsgList *insert_msg(MsgList *head, char* username, char* text);

MsgList *find_msg(MsgList *head, char* text);

void free_msgs(MsgList *head);

char *my_strstr(char *haystack,
                char *needle); //horspul
#endif
