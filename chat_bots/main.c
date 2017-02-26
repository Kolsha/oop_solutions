#include <stdio.h>
#include <stdlib.h>

#include "BaseUser/user.h"
#include "Girls/girl.h"
#include "Girls/CowardGirl.h"
#include "Utils/new.h"
#include "Utils/msg_list.h"
#include "Boys/boy.h"
#include "Boys/RichBoy.h"
#include "Utils/parser.h"
#include "Bots/spammer.h"

int main(int argc, char **argv)
{
    if(argc < 2){
        printf("Give me file please");
        return 1;
    }

    FILE *users = fopen (argv[1], "r");
    if(users == NULL){
        printf("Bad file!");
        return 1;
    }

    char * line = NULL;
    size_t len = 0;
    srand(time(NULL));

    MsgList *chat = insert_msg(NULL, "Kolsha", "Discuss started at now. Welcome");
    if(chat == NULL){
        printf("Ohh, bye!");

        fclose(users);

        if (line){

            free(line);
        }

        return 1;
    }

    while (getline(&line, &len, users) != -1) {

        ParserResult res = {NULL, r_none};

        if(parse_line(&res, line)){

            void *p = NULL;

            switch(res.role){
            case r_user:
                p = new(User, res.username, chat);
                break;
            case r_girl:
                p = new(Girl, res.username, chat, 100);
                break;
            case r_cowardgirl:
                p = new(CowardGirl, res.username, chat, 100);
                break;
            case r_boy:
                p = new(Boy, res.username, chat, 100);
                break;
            case r_richboy:
                p = new(RichBoy, res.username, chat, 100);
                break;
            default:
                p = new(Spammer, res.username, chat, 10);
                break;
            }

            free(res.username);

            if(p == NULL){
                continue;
            }

            type_msg(p);

            delete(p);


        }


    }

    free_msgs(chat);

    fclose(users);

    if (line){

        free(line);
    }

    return 0;
}
