#include "global.h"
#include "error.h"
#include "symbol.h"
#include "lexer.h"

#include <string.h>
#include <stdlib.h>

#define ISIZE 20

char inputbuff[ISIZE];
int  i_index;

char lexbuff[BSIZE];
int  lineno = 1;
int  tokenval = NONE;
int  inComment = 0;



void fill(int front){
    int i;
    if(front){
        for(i = 0; i < ISIZE/2-1; i++) {
            inputbuff[i] = fgetc(file);
        }
    } else {
        for(i = 0; i < ISIZE/2-1; i++) {
            inputbuff[ISIZE/2 + i] = fgetc(file);
        }
    }
}

void setupbuff() {
    i_index = -1; 
    
    inputbuff[ISIZE/2-1] = EOF; 
    inputbuff[ISIZE-1] = EOF;
    fill(1);
}

char nextchar() {
    i_index++;
    if(inputbuff[i_index] == EOF){
        int i;
        if(i_index == ISIZE/2-1){
            fill(0); 
            i_index++;
        } else if(i_index == ISIZE-1) { 
            fill(1);
            i_index = 0; 
        }
        // Else EOF, so return it anyway
    }

    /* 
    int j;
    for(j = 0; j < ISIZE; j++) {
        printf("%d ", inputbuff[j]);
    }
    printf("\n");
    */
    return inputbuff[i_index];
}

void ungetchar(char c) {
    inputbuff[i_index] = c;
    i_index--;
    if(i_index == ISIZE/2-1){
        i_index--;
    } else if(i_index == -1) { 
        i_index = ISIZE-1; 
    }
//    ungetc(c, file);
}

int lexan() 
{
    int t;
    while(1){
        t = nextchar();
        if(t=='\n') { // Count lines
            lineno++;
            // After a new line we are no longer in comment
            inComment = 0;
        } else if(t==' ' || t=='\t' || inComment) { // Ignore whitespace or comments
            ;
        } else if(isdigit(t)){ // Number
            //ungetchar(t);
            int b = 0;  
            while(isdigit(t)){
                lexbuff[b] = t;
                t = nextchar();
                b++;
                if(b >= BSIZE){
                    error("compiler error");
                }
            }
            lexbuff[b] = EOS;
            if (t != EOF) {
                ungetchar(t);
            }
            tokenval = atoi(lexbuff);
            //fscanf(file, "%d", &tokenval);
            return NUM;
        } else if(isalpha(t)) { // Word or identifier
            int p, b = 0;
            while (isalnum(t)) {
                lexbuff[b] = t;
                t = nextchar();
                b++;
                if(b >= BSIZE){
                    error("compiler error");
                }
            }
            lexbuff[b] = EOS;
            if (t != EOF) {
                ungetchar(t);
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
            fclose(file);
            return DONE;
        } else if (ispunct(t)) {
            int p, b = 0;
            switch(t){
                case '/':
                    t = nextchar();
                    if(t == '/') {
                        inComment = 1;
                        break;
                    } else {
                        ungetchar(t);
                        return DIV;
                    }
                case '%':
                    return MOD;
                case '-':
                    return MINUS;
                case '+':
                    return PLUS;
                case '=':
                    t = nextchar();
                    if(t != '=') {
                        ungetchar(t);
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
                    t = nextchar();
                    while (t != '"') {
                        lexbuff[b] = t;
                        t = nextchar();
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
            printf("%d\n", t);
            error("lexer");
            //tokenval = NONE
            //return t;
        }
    }
}

char* token_to_name(int tok){
    char* str = malloc(4);
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
        case 281:
            return "STRING";
        default:
            sprintf(str, "%d", tok);
            return str;
    }
}
