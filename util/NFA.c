#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "NFA.h"

static int state = -1;

int nextState() {
    return ++state;
}

struct NFA* create(){
    struct NFA *node = (struct NFA*) malloc(sizeof(struct NFA));
    node->initState = nextState();
    node->currFinalSize = 0;
    node->finalSize = 0;
    addFinalState(node, nextState());
    node->transitionSize = 0;
    node->currTransitionSize = 0;
    // addTransition(node, node->initState, letter, node->finalStates[0]);
    return node;
}

void freeNFA(struct NFA* nfa) { 
    int i;
    for(i = 0; i < nfa->currTransitionSize; i++){
        free(nfa->delta[i]);
    }
    free(nfa->delta);
    free(nfa->finalStates);
    free(nfa);
}

void removeFinalState(struct NFA* nfa, int f) {
    int i;
    for(i = 0; i < nfa->currFinalSize; i++){
        if(nfa->finalStates[i] == f){
            break;
        }
    }
    // Shift later enties down
    for(; i < nfa->currFinalSize-1; i++){
        nfa->finalStates[i] = nfa->finalStates[i+1];
    }
    nfa->currFinalSize--;
}

void addTransition(struct NFA* node, int a, char c, int b) {
    if(node->currTransitionSize == node->transitionSize){
        struct transition **newTransition = (struct transition**) calloc(node->transitionSize + 3, sizeof(struct transition*));
        int i;
        for(i = 0; i < node->transitionSize; i++) {
            newTransition[i] = node->delta[i];
        }
        free(node->delta);
        node->delta = newTransition;
        node->transitionSize += 3;
    }
    struct transition *newT = (struct transition*) calloc(1, sizeof(struct transition));
    newT->state = a;
    newT->match = c;
    newT->toState = b;
    node->delta[node->currTransitionSize++] = newT;
}

void addFinalState(struct NFA* node, int fs) {
    if(node->currFinalSize == node-> finalSize){
        int *newFS = (int*) calloc(node->finalSize + 3, sizeof(int));
        int i;
        for(i = 0; i < node->finalSize; i++) {
            newFS[i] = node->finalStates[i];
        }
        free(node->finalStates);
        node->finalStates = newFS;
        node->finalSize += 3;
    }
    node->finalStates[node->currFinalSize++] = fs;
}

void printNFA(struct NFA* nfa) {
    printf("Init: %d\n", nfa->initState);
    printf("Delta:\n");
    int i;
    for(i = 0; i < nfa->currTransitionSize; i++){
        struct transition *t = nfa->delta[i];
        if(isgraph(t->match)) {
            printf("(%d X '%c') -> %d\n", t->state, t->match, t->toState);
        } else {
            printf("(%d X %d) -> %d\n", t->state, t->match, t->toState);
        }
    }
    printf("Final:\n{ ");
    for(i = 0; i < nfa->currFinalSize; i++){
       printf("%d ", nfa->finalStates[i]);
    }
    printf("}\n");
}

int validate(struct NFA* nfa) {
    return nfa->initState;
}

// matches a OR b
struct NFA* orNFA(struct NFA* a, struct NFA* b) {
    struct NFA *or = create();
    int f = or->finalStates[0];
    // copy a and b transitions into or
    int i;
    for(i = 0; i < a->currTransitionSize; i++){
        struct transition *t = a->delta[i];
        addTransition(or, t->state, t->match, t->toState);
    }
    for(i = 0; i < b->currTransitionSize; i++){
        struct transition *t = b->delta[i];
        addTransition(or, t->state, t->match, t->toState);
    }
    // transition from final states to f
    for(i = 0; i < a->currFinalSize; i++){
        addTransition(or, a->finalStates[i], 0, f);
    }
    for(i = 0; i < b->currFinalSize; i++){
        addTransition(or, b->finalStates[i], 0, f);
    }
    
    // transitions from initial to a and b
    addTransition(or, or->initState, 0, a->initState);
    addTransition(or, or->initState, 0, b->initState);
    return or;
}

// matches a THEN b
struct NFA* thenNFA(struct NFA* a, struct NFA* b) {
    // copy b into a
    int i;
    for(i = 0; i < b->currTransitionSize; i++){
        struct transition *t = b->delta[i];
        addTransition(a, t->state, t->match, t->toState);
    }
    // transition from final of a to init of b
    for(i = 0; i < a->currFinalSize; i++){
        addTransition(a, a->finalStates[i], 0, b->initState);
    } 
    // set final states of a to those of b 
    for(i = 0; i < a->currFinalSize; i++){
        removeFinalState(a, a->finalStates[i]);
    }
    for(i = 0; i < b->currFinalSize; i++){
        addFinalState(a, b->finalStates[i]);
    }
    return a;
}

// matches a*
struct NFA* starNFA(struct NFA* a) {
    struct NFA *star = create();
    int f = star->finalStates[0];
    addTransition(star, star->initState, 0, f); 
    addTransition(star, star->initState, 0, a->initState);
    int i;
    // Copy a into star
    for(i = 0; i < a->currTransitionSize; i++){
        struct transition *t = a->delta[i];
        addTransition(star, t->state, t->match, t->toState);
    }
    // final of a to init of a
    for(i = 0; i < a->currFinalSize; i++) {
        addTransition(star, a->finalStates[i], 0, a->initState);
    }
    // final of a to final of star
    for(i = 0; i < a->currTransitionSize; i++){
        addTransition(star, a->finalStates[i], 0, f);
    }
    return star;
}

