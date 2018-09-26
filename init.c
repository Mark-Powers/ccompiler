#include "global.h"
#include "symbol.h"

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
}
