#include <stdlib.h>

#include "arraylist.h"

Arraylist* createAL(){
    Arraylist* al = calloc(1, sizeof(Arraylist));
    al->max = 0;
    al->size = 0;
    return al;
}

int get(Arraylist* al, int index){ 
    return al->list[index];
}

void add(Arraylist* al, int val){ 
    if(al->size == al->max){
        int* newList = calloc(al->size + 10, sizeof(int));
        int i;
        for(i = 0; i < al->size; i++){
            newList[i] = al->list[i];
        }
        free(al->list);
        al->list = newList;
        al->max += 10;
    }
    al->list[al->size] = val;
    al->size++;
}

void removeI(Arraylist* al, int index) {
    int i;
    for(i = index; i < al->size-1; i++){
        al->list[i] = al->list[i+1];
    }
    al->size--;
}

int contains(Arraylist* al, int value) {
    int i;
    for(i = 0; i < al->size; i++){
        if(get(al, i) == value) {
            return 1;
        }
    }
    return 0;
}

static int cmpfunc(const void *a, const void *b) {
    return ( *(int*)a - *(int*)b );
}

void sort(Arraylist* al) {
    qsort(al->list, al->size, sizeof(int), cmpfunc);
}

int size(Arraylist* al) {
    return al->size;
}
