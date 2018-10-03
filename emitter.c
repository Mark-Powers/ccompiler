#include "global.h"
#include "emitter.h"
#include "lexer.h"

#include <string.h>

int counter = 0;
char label[10];

void emit(int t, int tval)
{
    int i;
    switch(t) {
        case PLUS:
            fprintf(out, "+\n");
            break;
        case MINUS:
            fprintf(out, "-\n");
            break;
        case TIMES:
            fprintf(out, "*\n");
            break;
        case DIV:
            fprintf(out, "/\n");
            break;
        case MOD:
            fprintf(out, "%%\n");
            break;
        case NUM:
            fprintf(out, "push %d\n", tval);
            break;
        case ID:
            fprintf(out, "rvalue %s\n", symtable[tval].lexptr);
            break;
        case ASSIGN:
            fprintf(out, ":=\n");
            break;
        case IDLEFT:
            fprintf(out, "lvalue %s\n", symtable[tval].lexptr);
            break;
        case GOFALSE:
            fprintf(out, "gofalse %s\n", symtable[tval].lexptr);
            break;
        case LABEL:
            fprintf(out, "label %s\n", symtable[tval].lexptr);
            break;
       case GOTO:
            fprintf(out, "goto %s\n", symtable[tval].lexptr);
            break;
        case EQUAL:
            // Subtract top two items
            fprintf(out, "-\nnot\n");
            break;
        case PRINT:
            fprintf(out, "print\n");
            break;
        case STRING:
            i = strlen(symtable[tval].lexptr);
            do {
                fprintf(out, "push %d\n", symtable[tval].lexptr[i]);
                i--;
            } while (i >= 0);
            break;
        case PRINTSTR:
            fprintf(out, "printstr\n");
            break;
        default:
            fprintf(out, "token %s, tokenval %d\n", token_to_name(t), tval);
    }
}

char* nextLabel(){
    sprintf(label, "lbl%d", counter++);
    return label;
}

char* lastLabel(){
    return label;
}
