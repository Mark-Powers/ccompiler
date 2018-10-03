#include "global.h"
#include "error.h"
#include "symbol.h"
#include "lexer.h"

#include <string.h>
#include <stdlib.h>

#define ISIZE 10

char inputbuff[ISIZE];
int  i_index;
int  max_index; // Max index read into file
int  real_index; // Real i_index (no looping)

char lexbuff[BSIZE];
int  lineno = 1;
int  tokenval = NONE;
int  inComment = 0;
int  inError = 0;

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
    max_index += ISIZE/2-1;
}

void setupbuff() {
    i_index = -1; 
    real_index = 0;
    max_index = 0;
    
    inputbuff[ISIZE/2-1] = EOF; 
    inputbuff[ISIZE-1] = EOF;
    fill(1);

        int j;
        for(j = 0; j < ISIZE; j++) {
            printf("%d ", inputbuff[j]);
        }
        printf("\n");
}

char nextchar() {
    i_index++;
    int changed = 0;
    if(inputbuff[i_index] == EOF){
        changed = 1;
        int i;
        if(i_index == ISIZE/2-1) {
            if(real_index == max_index){
                fill(0); 
            }
            i_index++;
        } else if(i_index == ISIZE-1) { 
            if(real_index == max_index){
                fill(1); 
            }
            i_index = 0; 
        }
        // Else EOF, so return it anyway
    }

    if(debug && changed){ 
        int j;
        for(j = 0; j < ISIZE; j++) {
            fprintf(stderr, "%d ", inputbuff[j]);
        }
        fprintf(stderr, "\n");
    }
    printf("\t%d\n", inputbuff[i_index]); 

    real_index++;
    return inputbuff[i_index];
}

void ungetchar(char c) {
    printf("ungetting...\n");
    inputbuff[i_index] = c;
    i_index--;
    real_index--;
    if(i_index == ISIZE/2-1){
        i_index-=1;
        printf("\t\t%d\n", i_index);
    } else if(i_index == -1) { 
        i_index = ISIZE-2; 
    }
    // We shouldn't need to ungetc, because hopefully
    // our buffer is larger than the max lookahead needed
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
        } else if(inError) {
             while(t != ';'){
                t = nextchar();
                if(t=='\n') { 
                    lineno++;
                }
             }
             inError = 0;
        } else if(isdigit(t)){ // Number
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
            printf("Unknown character: %d\n", t);
            error("lexer");
            //tokenval = NONE
            //return t;
        }
    }
}

void skipstatement() {
    inError = 1;
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
