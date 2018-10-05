#include <stdlib.h>

#include "NFA.h"

struct NFA* NFAalloc(){
    static int state = 0;
    struct NFA *node = (struct NFA*) malloc(sizeof(struct NFA));
    node->state = state++;
    // A single node NFA is its own finish state
    node->finishState = node;
    node->edgeSize = 0;
    node->currEdges = 0;
    return node;
}

void createEdge(struct NFA* n1, struct NFA* n2, char match) {
    n1 = n1->finishState;
    if(n1->currEdges == n1->edgeSize){
        // Create new edge array with 2 extra spots
        int newSize = n1->currEdges + 2;
        struct edge **edges = (struct edge**) malloc(sizeof(struct edge) * newSize);
        int i;
        for(i = 0; i < n1->currEdges; i++) {
            edges[i] = n1->edges[i];
        }
        n1->edges = edges;
        n1->edgeSize = newSize;
    }

    struct edge newEdge;
    newEdge.match = match;
    newEdge.next = n2;
    n1->edges[n1->currEdges] = &newEdge;
    n1->currEdges++;
}

// Creates an edge between n1's finish state and n2's initial state
void createEndEdge(struct NFA* n1, struct NFA* n2, char match) {
    createEdge(n1->finishState, n2, match);
    // How to tell if finished?
    n1->finishState = n2;
}

struct NFA* create(char letter) {
    struct NFA *init = NFAalloc();
    struct NFA *end = NFAalloc();
    createEdge(init, end, letter);
    return init;
}

struct NFA* or(struct NFA* n1, struct NFA*n2) {
    struct NFA *init = NFAalloc();
    struct NFA *end = NFAalloc();
    createEdge(init, n1, 0);
    createEdge(init, n2, 0);
    createEndEdge(n1, end, 0);
    createEndEdge(n2, end, 0);
    return init;
}

struct NFA* then(struct NFA* n1, struct NFA*n2) {
    createEndEdge(n1, n2, 0);
    return n1;
}

struct NFA* kline(struct NFA* n1) {
    struct NFA *init = NFAalloc();
    struct NFA *end = NFAalloc();
    createEdge(init, end, 0);
    createEdge(init, n1, 0);
    createEndEdge(n1, end, 0);
    return init;
}
