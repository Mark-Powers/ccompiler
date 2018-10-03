#include "init.h"
#include "global.h"
#include "util/symbol.h"
#include "lexer/io.h"

struct entry keywords[] = {
    "mod", MOD,
    "if", IF,
    "then", THEN,
    "while", WHILE,
    "do", DO,
    "print", PRINT,
    0, 0
};

void init()
{
    struct entry *p;
    for(p = keywords; p->token; p++){
        insert(p->lexptr, p->token);
    }

    setupbuff();
}
