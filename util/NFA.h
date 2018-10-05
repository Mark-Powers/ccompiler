struct NFA* NFAalloc();
struct NFA* create(char);
void createEdge(struct NFA*, struct NFA*, char);
void createEndEdge(struct NFA*, struct NFA*, char);


struct NFA {
    int state;
    struct edge **edges;
    int edgeSize;
    int currEdges;
    struct NFA *finishState;
};

struct edge {
    char match;
    struct NFA *next;
};
