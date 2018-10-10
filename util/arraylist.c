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

int pop(Arraylist* al) {
    int t = al->list[al->size-1];
    removeI(al, al->size-1);
    return t;
}

void insertAt(Arraylist* al, int i, int val) {
    al->list[i] = val;
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

int equalAL(Arraylist *al1, Arraylist *al2) {
    int i;
    if(al1->size != al2->size){
        return 0;
    }
    for(i = 0; i < al1->size; i++){
        if(al1->list[i] != al2->list[i]){
            return 0;
        }
    }
    return 1;
}

