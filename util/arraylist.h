#ifndef ARRAYLIST_H
#define ARRAYLIST_H

typedef struct Arraylist {
    int *list;
    int size;
    int max;
} Arraylist;

Arraylist* createAL();
int get(Arraylist*, int);
void add(Arraylist*, int);
int pop(Arraylist*);
void insertAt(Arraylist*, int, int);
void removeI(Arraylist*, int);
int contains(Arraylist*, int);
void sort(Arraylist*);
int size(Arraylist*);

#endif // ARRAYLIST_H

