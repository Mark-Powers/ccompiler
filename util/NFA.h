int nextState();

struct NFA* create();
// a X c -> b
void addTransition(struct NFA*, int a, char c, int b);
void addFinalState(struct NFA*, int);
void removeFinalState(struct NFA* nfa, int f);
void printNFA(struct NFA*);
int validate(struct NFA*);
struct NFA* orNFA(struct NFA*, struct NFA*);
struct NFA* thenNFA(struct NFA*, struct NFA*);
struct NFA* starNFA(struct NFA*);

struct NFA {
    int initState;
    struct transition **delta;
    int transitionSize;
    int currTransitionSize;
    int *finalStates;
    int finalSize;
    int currFinalSize;
};

struct transition {
    int state;
    char match;
    int toState;
};

