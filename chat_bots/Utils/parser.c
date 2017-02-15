#include <stdlib.h>
#include <string.h>
#include "parser.h"

int parse_line(ParserResult *res, char *line){

    if(res == NULL || line == NULL){
        return 0;
    }
    res->role = r_none;
    size_t pos = 0;
    while(line[pos] != ' ' && line[pos] != '\0'){
        pos++;
    }

    res->username = malloc(sizeof(char) * pos + 1);
    if(res->username == NULL){
        return 0;
    }

    if(memcpy(res->username, line, sizeof(char) * pos) == NULL){
        free(res->username);
        return 0;
    }

    pos++;
    res->username[pos] = '\0';

    char role_char = tolower(line[pos]);

    switch(role_char){
    case 'u':
        res->role = r_user;
        break;
    case 'r':
        res->role = r_richboy;
        break;
    case 'g':
        res->role = r_girl;
        break;
    case 'b':
        res->role = r_boy;
        break;
    case 'c':
        res->role = r_cowardgirl;
        break;
    default:
        res->role = r_spammer;
    }

    return 1;

}

