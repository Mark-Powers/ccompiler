#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "NFA.h"
#include "arraylist.h"

static int state = -1;

int nextState() {
    return ++state;
}

struct NFA* create(){
    struct NFA *node = (struct NFA*) malloc(sizeof(struct NFA));
    node->initState = nextState();
    node->finalStates = createAL();
    addFinalState(node, nextState());
    node->transitionSize = 0;
    node->currTransitionSize = 0;
    return node;
}

void freeNFA(struct NFA* nfa) { 
    int i;
    for(i = 0; i < nfa->currTransitionSize; i++){
        free(nfa->delta[i]);
    }
    free(nfa->delta);
    // TODO free arraylist
    free(nfa);
}

void removeFinalState(struct NFA* nfa, int f) {
    int i;
    for(i = 0; i < size(nfa->finalStates); i++){
        if(get(nfa->finalStates, i) == f){
            removeI(nfa->finalStates, i);
        }
    }
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

void addFinalState(struct NFA* nfa, int fs) {
    if(!contains(nfa->finalStates, fs)) {
        add(nfa->finalStates, fs);
    }
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
    for(i = 0; i < size(nfa->finalStates); i++){
       printf("%d ", get(nfa->finalStates, i));
    }
    printf("}\n");
}

void copyTransitions(struct NFA* from, struct NFA* to) {
    int i;
    for(i = 0; i < from->currTransitionSize; i++){
        struct transition *t = from->delta[i];
        addTransition(to, t->state, t->match, t->toState);
    }    
}

void addTransitionsFromFinalStates(struct NFA* to, struct NFA* f, int s) {
    int i;
    for(i = 0; i < size(f->finalStates); i++){
        addTransition(to, get(f->finalStates, i), 0, s);
    }
}

// matches a OR b
struct NFA* orNFA(struct NFA* a, struct NFA* b) {
    struct NFA *or = create();
    int f = get(or->finalStates, 0);
    copyTransitions(a, or);
    copyTransitions(b, or);
    // transition from final states of a and b to f
    addTransitionsFromFinalStates(or, a, f);
    addTransitionsFromFinalStates(or, b, f);
    
    // transitions from initial to a and b
    addTransition(or, or->initState, 0, a->initState);
    addTransition(or, or->initState, 0, b->initState);
    return or;
}

// matches a THEN b
struct NFA* thenNFA(struct NFA* a, struct NFA* b) {
    copyTransitions(b, a);
    // transition from final of a to init of b
    addTransitionsFromFinalStates(a, a, b->initState);
    // set final states of a to those of b 
    int i;
    for(i = 0; i < size(a->finalStates); i++){
        removeFinalState(a, get(a->finalStates, i));
    }
    for(i = 0; i < size(b->finalStates); i++){
        addFinalState(a, get(b->finalStates, i));
    }
    return a;
}

// matches a*
struct NFA* starNFA(struct NFA* a) {
    struct NFA *star = create();
    int f = get(star->finalStates, 0);
    addTransition(star, star->initState, 0, f); 
    addTransition(star, star->initState, 0, a->initState);
    
    copyTransitions(a, star);
    // final of a to init of a
    addTransitionsFromFinalStates(star, a, a->initState);
    // final of a to final of star
    addTransitionsFromFinalStates(star, a, f);
    return star;
}

int validate(struct NFA *n, char* string) {
    int state = n->initState;
    int i = 0;
    while(string[i] != '\0') {
        state = move(n, state, string[i]);
        if(state == -1){ 
            return 0;
        }
        i++;
    }
    return contains(n->finalStates, state);
}

int move(struct NFA* n, int s, char m) {
    int i;
    struct transition *t;
    for(i = 0; i < n->currTransitionSize; i++){
        t = n->delta[i];
        if(t->state == s && t->match == m){
            return t->toState;
        }
    }
    return -1;
}


Arraylist* eclosureOne(struct NFA* n, int state) {
    Arraylist* al = createAL();
    add(al, state);
    return eclosureMany(n, al);
}

Arraylist* eclosureMany(struct NFA* n, Arraylist* states) {
    int i;
    // Create stack of all given states
    Arraylist *stack = states;
    //add(stack, state);
    // Closure starts as just given state.
    Arraylist *al = createAL();
    add(al, state);
    // While stack is not empty
    while(size(stack) > 0) {
        int t = pop(al);
        // For each transition
        for(i = 0; i < n->currTransitionSize; i++){
            struct transition *transition;
            transition = n->delta[i];
            // if transition out of t on e that isn't already in the closure 
            if(transition->state == t && transition->match == 0 && !contains(al, transition->toState)){
                add(al, transition->toState);
                add(stack, transition->toState);
            }
        }
    }
    return al;
}
/*
struct Dstate {
    Arraylist* states;
    int stateLength
    int mark;
};

static struct Dstate* getUnmarked(struct Dstate** dstates, int n) {
    int i;
    for(i = 0; i < n; i++) {
        if(dstates[i]->mark){
            return 1;
        }
    }
    return 0;
}

static int contains(int[] a, int n, int s){
    int i;
    for(i = 0; i < n; i++){
        if(a[i] == s){
            return 1;
        }
    }
    return 0;
}

struct NFA* toDFA(struct NFA* n) {
    struct Dstate** dstates = (struct Dstate**) calloc(10, sizeof(struct Dstate*));
    int dstatesLength = 0;
   
    // While there is an unmarked T
    while(getUnmarked(dstates, dstatesLength)){
        struct Dstate* t = getUnmarked(dstates, dstatesLength);
        // For each input symbol a off of a state in T
        int i;
        for(i = 0; i < n->currTransitionSize; i++){
            if(contains(t->states, t->stateLength, n->delta[i]->state)) {
                eclosure(move(T, t->match));
                
                // U := eclosure(move(T, a));
                // if U is not in Dstates then
                // add U as an unmarked state to Dstates:
                // Dtran(T, a) ;= U

            }
        }

    }

    return n;
}
*/
