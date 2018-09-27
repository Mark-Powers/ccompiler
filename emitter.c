#include "global.h"
#include "emitter.h"
#include "lexer.h"

int counter = 0;
char label[10];

void emit(int t, int tval)
{
    //printf("token %s, tokenval %d\n", token_to_name(t), tval);
    switch(t) {
        case PLUS:
            printf("+\n");
            break;
        case MINUS:
            printf("-\n");
            break;
        case TIMES:
            printf("*\n");
            break;
        case DIV:
            printf("/\n");
            break;
        case MOD:
            printf("%%\n");
            break;
        case NUM:
            printf("push %d\n", tval);
            break;
        case ID:
            printf("rvalue %s\n", symtable[tval].lexptr);
            break;
        case ASSIGN:
            printf(":=\n");
            break;
        case IDLEFT:
            printf("lvalue %s\n", symtable[tval].lexptr);
            break;
        case GOFALSE:
            printf("gofalse %s\n", symtable[tval].lexptr);
            break;
        case LABEL:
            printf("label %s\n", symtable[tval].lexptr);
            break;
       case GOTO:
            printf("goto %s\n", symtable[tval].lexptr);
            break;
        case EQUAL:
            // Subtract top two items
            printf("-\nnot\n");
            break;
        case PRINT:
            printf("print\n");
            break;
        default:
            printf("token %s, tokenval %d\n", token_to_name(t), tval);
    }
}

char* nextLabel(){
    sprintf(label, "lbl%d", counter++);
    return label;
}

char* lastLabel(){
    return label;
}
