#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdio.h>
#include <ctype.h>

#define SYMMAX  100
#define BSIZE   128
#define NONE    -1
#define EOS     '\0'

#define NUM     256
#define DIV     257
#define MOD     258
#define ID      259
#define DONE    260
#define ASSIGN  261
#define IF      262
#define THEN    263
#define WHILE   264
#define DO      265
#define BEGIN   266
#define END     267
#define PLUS    268
#define MINUS   269
#define EQUAL   270
#define SEMIC   271
#define TIMES   272
#define LPAREN  273
#define RPAREN  274
#define IDLEFT  275
#define PRINT   276
#define GOTO    277
#define LABEL   278
#define GOFALSE 279
#define PRINTSTR 280
#define STRING  281

int tokenval;
int lineno;
int debug;
FILE *file;
FILE *out;

struct entry {
    char *lexptr;
    int  token;
};

struct entry symtable[SYMMAX];

#endif // GLOBAL_H

