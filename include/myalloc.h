#ifndef MYALLOC_H
#define MYALLOC_H
typedef struct BLOCK{
    void* start;
    int length;
    struct BLOCK* next; 
} BLOCK;

extern BLOCK* freeHead;
extern void* heapStart;
extern short heapInitialized;
extern void* heapEnd;
void* myalloc(int size);
void  myfree(void* ptr);
#endif