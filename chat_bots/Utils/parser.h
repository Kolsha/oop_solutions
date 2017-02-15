#ifndef PARSER_H
#define PARSER_H






typedef enum {r_user, r_girl, r_boy, r_richboy, r_spammer, r_cowardgirl, r_none} roles;

typedef struct _ParserResult {
    char *username;
    roles role;
} ParserResult;

int parse_line(ParserResult *res, char *line);


#endif // PARSER_H

