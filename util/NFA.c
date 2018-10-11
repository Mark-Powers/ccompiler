#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "NFA.h"
#include "regex.h"
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
    printf("Init:\t%d\n", nfa->initState);
    printf("Delta:\n");
    int i;
    for(i = 0; i < nfa->currTransitionSize; i++){
        struct transition *t = nfa->delta[i];
        if(isgraph(t->match)) {
            printf("\t(%d X '%c') -> %d\n", t->state, t->match, t->toState);
        } else {
            printf("\t(%d X %d) -> %d\n", t->state, t->match, t->toState);
        }
    }
    printf("Final:\n\t{ ");
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
        if(t->state == s) {
            if(t->match == m ||
                (t->match == ANY_ALPHANUM && isalnum(m)) ||
                (t->match == ANY_ALPHA && isalpha(m)) ||
                (t->match == ANY_NUM && isdigit(m))){
                return t->toState;
            }
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
    Arraylist *stack = createAL();
    for(i = 0 ; i < size(states); i++){
        add(stack, get(states, i));
    }
    // Closure starts as just given states.
    Arraylist *al = createAL();
    for(i = 0 ; i < size(states); i++){
        add(al, get(states, i));
    }
    // While stack is not empty
    while(size(stack) > 0) {
        int t = pop(stack);
        // For each transition
        for(i = 0; i < n->currTransitionSize; i++){
            struct transition *transition;
            transition = n->delta[i];
            // if transition out of t on e that isn't already in the closure 
            if(transition->state == t && transition->match == 0 && !contains(al, transition->toState)){
                add(al, transition->toState);
                // This is an issue
                add(stack, transition->toState);
            }
        }
    }
    sort(al);
    return al;
}

struct Dstate {
    Arraylist* states;
    int mark;
};

struct Dtran {
    Arraylist* t;
    char a;
    Arraylist* u;
};

static struct Dstate** addDstate(struct Dstate** dstates, int* n, int* max, Arraylist* al, int mark) {
    struct Dstate* newDstate = (struct Dstate*) calloc(1, sizeof(struct Dstate));
    newDstate->states = al;
    newDstate->mark = mark;
    if(*n == *max){
        struct Dstate** newDstates = (struct Dstate**) calloc((*max)*2, sizeof(struct Dstate*));
        int i;
        for(i = 0; i < *n; i++){
            newDstates[i] = dstates[i];
        }
        // TODO free dstates
        dstates = newDstates;
        (*max) *= 2;
    }
    dstates[(*n)++] = newDstate;
    return dstates;
}

static struct Dtran** addDtran(struct Dtran** dtrans, int* n, int* max, Arraylist* t, char a, Arraylist* u) {
    struct Dtran* newDtran = (struct Dtran*) calloc(1, sizeof(struct Dtran));
    newDtran->t = t;
    newDtran->a = a;
    newDtran->u = u;
    if(*n == *max){
        struct Dtran** newDtrans = (struct Dtran**) calloc((*max)*2, sizeof(struct Dtran*));
        int i;
        for(i = 0; i < *n; i++){
            newDtrans[i] = dtrans[i];
        }
        // TODO free dtrans
        dtrans = newDtrans;
        (*max) *= 2;
    }
    dtrans[(*n)++] = newDtran;
    return dtrans;
}

struct Dstate** dstateTranslations;
static int getTranslatedState(Arraylist* ds){
    // dstate is list, mark is new state
    static int dstatesLength2 = 0;
    static int dstatesMax2 = 0;
    // If state is already known, return its new state value (mark)
    int i;
    for(i = 0; i < dstatesLength2; i++){
        if(equalAL(dstateTranslations[i]->states, ds)){
            return dstateTranslations[i]->mark;
        }
    }
    // Otherwise add it and use the new mark 
    int t = nextState();
    dstateTranslations = addDstate(dstateTranslations, &dstatesLength2, &dstatesMax2, ds, t);
    return t; 
}

static struct Dstate* getUnmarked(struct Dstate** dstates, int n) {
    int i;
    for(i = 0; i < n; i++) {
        // Return the first unmarked state
        if(!dstates[i]->mark){
            return dstates[i];
        }
    }
    // No unmarked state
    return NULL;
}

static int containsDstate(struct Dstate** dstates, int n, Arraylist* al){
    int i;
    for(i = 0; i < n; i++) {
        if(equalAL(dstates[i]->states, al)) {
            return 1;
        }
    }
    return 0;
}

Arraylist* getMoves(struct NFA* nfa, Arraylist* states, char a) {
    Arraylist* al = createAL();
    int i;
    for(i = 0; i < nfa->currTransitionSize; i++){
        struct transition* t = nfa->delta[i];
        if(contains(states, t->state) && t->match == a) {
            add(al, t->toState);
        }
    }
    return al;
}

struct NFA* toDFA(struct NFA* n) {
    struct Dtran** dtrans = (struct Dtran**) calloc(10, sizeof(struct Dtran*));
    int dtransLength = 0;
    int dtransMax = 0;

    // Initially dstates only has eclosure of the initial state
    struct Dstate** dstates = (struct Dstate**) calloc(10, sizeof(struct Dstate*));
    int dstatesLength = 0;
    int dstatesMax = 0;
    dstates = addDstate(dstates, &dstatesLength, &dstatesMax, eclosureOne(n, n->initState), 0);
   
    // While there is an unmarked state t
    while(getUnmarked(dstates, dstatesLength)){
        struct Dstate* t = getUnmarked(dstates, dstatesLength);
        // Mark t
        t->mark = 1;
        // For each input symbol a from a state in t
        int i;
        for(i = 0; i < n->currTransitionSize; i++){
            char a = n->delta[i]->match;
            if(a != 0 && contains(t->states, n->delta[i]->state)) {
                Arraylist* u = eclosureMany(n, getMoves(n, t->states, a));
                sort(u);
                // if U is not in Dstates then
                if(!containsDstate(dstates, dstatesLength, u)) {
                    // add U as an unmarked state to Dstates:
                    dstates = addDstate(dstates, &dstatesLength, &dstatesMax, u, 0);
                }
                // Dtran(T, a) ;= U
                dtrans = addDtran(dtrans, &dtransLength, &dtransMax, t->states, a, u);

            }
        }
    }
   
    struct NFA* dfa = create();
    int firstState = getTranslatedState(eclosureOne(n, n->initState)); 
    // Clear final and ending state;
    dfa->initState = firstState;
    removeFinalState(dfa, get(dfa->finalStates, 0));
    // PARSE dtrans
    int i,j;
    for(i = 0 ; i < dtransLength; i++){
        struct Dtran *d = dtrans[i];

        int state = getTranslatedState(d->t);
        int toState = getTranslatedState(d->u);
        addTransition(dfa, state, d->a, toState);

        for(j = 0; j < size(n->finalStates); j++){
            int f = get(n->finalStates, j);
            if(contains(d->u, f)){
                addFinalState(dfa, toState);
                //f = toState;
            }
        } 
    }
    return dfa;
}

