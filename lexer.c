#include "global.h"
#include "error.h"
#include "symbol.h"

#include <string.h>

char lexbuff[BSIZE];
int  lineno = 1;
int  tokenval = NONE;
int  inComment = 0;

int lexan() 
{
    int t;
    while(1){
        t = getchar();
        if(t=='\n') { // Count lines
            lineno++;
            // After a new line we are no longer in comment
            inComment = 0;
        } else if(t==' ' || t=='\t' || inComment) { // Ignore whitespace or comments
            ;
        } else if(isdigit(t)){ // Number
            ungetc(t, stdin);
            scanf("%d", &tokenval);
            return NUM;
        } else if(isalpha(t)) { // Word or identifier
            int p, b = 0;
            while (isalnum(t)) {
                lexbuff[b] = t;
                t = getchar();
                b++;
                if(b >= BSIZE){
                    error("compiler error");
                }
            }
            lexbuff[b] = EOS;
            if (t != EOF) {
                ungetc(t, stdin);
            }
            // Checks for reserved word
            if(strcmp(lexbuff, "if") == 0){
                return IF;    
            } else if(strcmp(lexbuff, "then") == 0){
                return THEN;    
            } else if(strcmp(lexbuff, "while") == 0){
                return WHILE;    
            } else if(strcmp(lexbuff, "do") == 0){
                return DO;
            } else if(strcmp(lexbuff, "print") == 0){
                return PRINT;
            }
            // Otherwise must be an identifier
            p = lookup(lexbuff);
            if (p == 0){
                p = insert(lexbuff, ID);
            }
            tokenval = p;
            return symtable[p].token;
        } else if (t == EOF) {
            return DONE;
        } else if (ispunct(t)) {
            int p, b = 0;
            switch(t){
                case '/':
                    t = getchar();
                    if(t == '/') {
                        inComment = 1;
                        break;
                    } else {
                        ungetc(t, stdin);
                        return DIV;
                    }
                case '%':
                    return MOD;
                case '-':
                    return MINUS;
                case '+':
                    return PLUS;
                case '=':
                    t = getchar();
                    if(t != '=') {
                        ungetc(t, stdin);
                        return ASSIGN;
                    }
                    return EQUAL;
                case '*':
                    return TIMES;
                case ';':
                    return SEMIC;
                case '{':
                    return BEGIN;
                case '}':
                    return END;
                case '(':
                    return LPAREN;
                case ')':
                    return RPAREN;
                case '"':
                    t = getchar();
                    while (t != '"') {
                        lexbuff[b] = t;
                        t = getchar();
                        b++;
                        if(b >= BSIZE){
                            error("compiler error");
                        }
                    }
                    lexbuff[b] = EOS;
                    p = lookup(lexbuff);
                    if (p == 0){
                        p = insert(lexbuff, STRING);
                    }
                    tokenval = p;
                    return STRING;
            }
        } else {
            error("syntax error: unexpected token");
            //tokenval = NONE
            //return t;
        }
    }
}

char* token_to_name(int tok){
    switch(tok){
        case 256:
            return "NUM";
        case 257:
            return "DIV";
        case 258:
            return "MOD";
        case 259:
            return "ID";
        case 260:
            return "DONE";
        case 261:
            return "ASSIGN";
        case 262:
            return "IF";
        case 263:
            return "THEN";
        case 264:
            return "WHILE";
        case 265:
            return "DO";
        case 266:
            return "BEGIN";
        case 267:
            return "END";
        case 268:
            return "PLUS";
        case 269:
            return "MINUS";
        case 270:
            return "EQUAL";
        case 271:
            return "SEMIC";
        case 272:
            return "TIMES";
        case 273:
            return "LPAREN";
        case 274:
            return "RPAREN";
        case 275:
            return "IDLEFT";
        case 276:
            return "PRINT";
    }
}
