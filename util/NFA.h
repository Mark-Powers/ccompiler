#include "arraylist.h"

int nextState();

struct NFA* create();
// a X c -> b
void addTransition(struct NFA*, int a, char c, int b);
void addFinalState(struct NFA*, int);
void removeFinalState(struct NFA* nfa, int f);
void printNFA(struct NFA*);
void copyTransitions(struct NFA*, struct NFA* );
void addTransitionsFromFinalStates(struct NFA*, struct NFA* , int);
struct NFA* orNFA(struct NFA*, struct NFA*);
struct NFA* thenNFA(struct NFA*, struct NFA*);
struct NFA* starNFA(struct NFA*);
int validate(struct NFA*, char*);
int move(struct NFA*, int, char);
struct NFA* toDFA(struct NFA*);
Arraylist* eclosureOne(struct NFA*, int);
Arraylist* eclosureMany(struct NFA*, Arraylist*);

struct NFA {
    int initState;
    struct transition **delta;
    int transitionSize;
    int currTransitionSize;
    Arraylist *finalStates;
};

struct transition {
    int state;
    char match;
    int toState;
};

