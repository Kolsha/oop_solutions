#include <stdio.h>
#include "user.h"
#include "girl.h"
#include "new.h"
#include "msg_list.h"

int main(int argc, char **argv)
{
    srand(time(NULL));
    MsgList *chat = insert_msg(NULL, "Kolsha", "Discuss started at now. Welcome");
    if(chat == NULL){
        printf("Ohh, bye!");
        return 1;
    }
    void *p;
    while (*++argv) {
        switch (**argv) {
        case 'p':
            p = new(User, "TEst", chat);
            break;
        case 'c':
            p = new(Girl, "Bich", chat, 100);
            break;
        default:
            continue;
        }

        type_msg(p);

        delete(p);
    }

    return 0;
}
