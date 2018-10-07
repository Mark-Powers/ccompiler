#include <string.h>
#include <ctype.h>
#include <stdlib.h> 

#include "regex.h"
#include "NFA.h"
#include "error.h"

#define TERM 1
#define OR   2
#define STAR 3
#define LPAREN 4
#define RPAREN 5

#define ALPHA -10
#define NUM   -11

static struct NFA **stack;
static unsigned int sp;

struct NFA* compileRE(char* pattern) {
    stack = (struct NFA**) calloc(strlen(pattern), sizeof(struct NFA*));
    sp = 0;
    unsigned int i;
    for(i = 0; i < strlen(pattern); ) {
        i+=next(pattern+i);
    }
    struct NFA* temp;
    for(i = 1; i < sp; i++){
        temp = stack[--sp];
        temp = thenNFA(stack[--sp], temp);
        stack[sp++] = temp;
    }
    return NULL;
}

int next(char* pattern) {
    int i;
    struct NFA* temp;
    switch(pattern[0]){
        case '(':
            i = 1;
            while(pattern[i] != ')') {
                i += next(pattern+i);  
            }
            return i+1;
        case '*':
            temp = stack[--sp];
            temp = starNFA(temp);
            stack[sp++] = temp;
            return 1;
        case '|':
            i = next(pattern+1);
            temp = orNFA(stack[sp-2], stack[sp-1]);
            sp-=2;
            stack[sp++] = temp;
            return i+1;
        default:
            temp = create();
            addTransition(temp, temp->initState, pattern[0], temp->finalStates[0]);
            stack[sp++] = temp;
            return 1;
    }
}

