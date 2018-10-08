typedef struct Arraylist {
    int *list;
    int size;
    int max;
} Arraylist;

Arraylist* createAL();
int get(Arraylist*, int);
void add(Arraylist*, int);
void removeI(Arraylist*, int);
int contains(Arraylist*, int);
void sort(Arraylist*);
int size(Arraylist*);

