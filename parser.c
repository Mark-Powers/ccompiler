#include "global.h"
#include "parser.h"
#include "lexer.h"
#include "emitter.h"
#include "error.h"
#include "symbol.h"

int lookahead;

void parse(){
    lookahead = lexan();
    if(debug){
        printf("->%s\n", token_to_name(lookahead));
    }
    while(lookahead != DONE) {
        stmt_list();
    }
}

void stmt(){
    char* lbl;
    switch(lookahead) {
        case BEGIN: // Code Block
            match(BEGIN);
            stmt_list();
            match(END);
            break;
        case ID: // Assignment
            emit(IDLEFT, tokenval);
            match(ID);
            match(ASSIGN);
            expr();
            emit(ASSIGN, NONE);
            break;
        case PRINT: // Output
            match(PRINT);
            expr();
            emit(PRINT, NONE);
            break;
        case IF: // If statement
            match(IF);
            expr();
            lbl = nextLabel();
            int p;
            p = lookup(lbl);
            if(p == 0){
                p = insert(lbl, IF);
            }
            emit(GOFALSE, p);
            match(THEN);
            stmt();
            emit(LABEL, p);
            break;
        case WHILE: // While loop
            match(WHILE);
            lbl = nextLabel();
            int p1;
            p1 = lookup(lbl);
            if(p1 == 0){
                p1 = insert(lbl, IF);
            }
            emit(LABEL, p1), 
            expr();
            lbl = nextLabel();
            int p2;
            p2 = lookup(lbl);
            if(p2 == 0){
                p2 = insert(lbl, IF);
            }
            match(DO);
            emit(GOFALSE, p2);
            stmt();
            emit(GOTO, p1);
            emit(LABEL, p2);
            break;
        default:
            syntaxError("", "statement", token_to_name(lookahead));
    } 
}

void stmt_list(){
    stmt();
    fprintf(stderr, "NEXT STATEMENT\n");
    while(lookahead != DONE){
        stmt(); 
        match(SEMIC);
        fprintf(stderr, "NEXT STATEMENT\n");
    }
}

void expr()
{
    int t;
    term();
    while(1) {
        if(lookahead==PLUS || lookahead==MINUS){
            t = lookahead;
            match(lookahead);
            term();
            emit(t, NONE);
            continue;
        } else {
            return;
        }
    }
        
}

void term()
{
    int t;
    factor();
    while(1) {
        if(lookahead==TIMES ||  lookahead==DIV || lookahead==MOD){
            t = lookahead;
            match(lookahead);
            factor();
            emit(t, NONE);
            continue;
        } else {
            return;
        }
    }
}

void factor()
{
    switch(lookahead) {
        case LPAREN:
            match(LPAREN);
            expr();
            match(RPAREN);
            break;
        case NUM:
            emit(NUM, tokenval);
            match(NUM);
            break;
        case ID:
            emit(ID, tokenval);
            match(ID);
            break;
        default:
            printf("%s\n", token_to_name(lookahead));
            error("syntax error: factor");
    } 
}

void match(int t)
{
    if(lookahead == t) {
        lookahead = lexan();
        if(debug){
            printf("->%s\n", token_to_name(lookahead));
        }
    } else {
        syntaxError("match", token_to_name(t),token_to_name(lookahead));
    }
}
