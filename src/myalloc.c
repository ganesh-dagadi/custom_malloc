#include "myalloc.h"
#include <unistd.h>
#include <stdio.h>

BLOCK* freeHead = NULL;
void* heapStart = NULL;
short heapInitialized = 0;
void* heapEnd = NULL;

void* getNewMemory(int size) {
    void* newMem = sbrk(size);
    heapEnd = newMem;
    return newMem;
}
void initializeMemory() {
    heapStart = getNewMemory(0);
    heapInitialized = 1;
}
void* allocateFreeBlock(BLOCK* currFreePtr, BLOCK* prevFreeBlock, int size) {
    void* nextExistMetaStart = currFreePtr->next;
    if (currFreePtr->length > (size + sizeof(BLOCK))){
        // create a new free block
        BLOCK* newFreeBlock = (BLOCK*)((char*)currFreePtr->start + size);
        newFreeBlock->start = (void*)((char*)newFreeBlock + sizeof(BLOCK));
        newFreeBlock->length = currFreePtr->length - (size + sizeof(BLOCK));
        if(prevFreeBlock != NULL) prevFreeBlock->next = newFreeBlock;
        else freeHead = newFreeBlock;
        newFreeBlock->next = nextExistMetaStart;
    } else if (currFreePtr->length == size){
        if(prevFreeBlock == NULL) freeHead = currFreePtr->next;
        else prevFreeBlock = currFreePtr->next;
    } else if (heapEnd == (void*)((char*)currFreePtr + currFreePtr->length + sizeof(BLOCK))){ //last block of heap
        //release wasted memory
        heapEnd = sbrk(-(currFreePtr->length - size));
    }
    currFreePtr->length = size;
    currFreePtr->next = NULL;
    return currFreePtr->start;
}
void* myalloc(int size) {
    if ((size % 8) != 0) {
        size += 8; // maintain blocks of size of multiple 8
        size -= size % 8;
    }
    if (heapInitialized == 0) {
        initializeMemory();
    }
    BLOCK* currBlockPtr = freeHead;;
    BLOCK* prevFreeBlock = NULL;;
    // find free block and allocate it
    while (currBlockPtr != NULL) {
        if(currBlockPtr->length >= size) {
            return allocateFreeBlock(currBlockPtr, prevFreeBlock, size);
        }
        prevFreeBlock = currBlockPtr;
        currBlockPtr = currBlockPtr->next;
    }
    //no free block found, allocate new memory and add to end of alloc
    BLOCK* newBlockMeta = (BLOCK*)getNewMemory(sizeof(BLOCK));
    newBlockMeta->length = size;
    newBlockMeta->next = NULL;
    newBlockMeta->start = getNewMemory(size);
    return newBlockMeta->start;
}

// void myfree(void* ptr) {
//     // set into correct position based on memory address of Free LinkList
//     BLOCK* startBlock = (BLOCK*)((char*)ptr - sizeof(BLOCK));
//     startBlock->next = NULL;
//     if (freeHead == NULL) {
//         freeHead = startBlock;
//         return;
//     }
//     BLOCK* currFreePtr = freeHead;
//     BLOCK* prevFreePtr = NULL;
//     while(currFreePtr->next != NULL) {
//         if (currFreePtr->next > startBlock) {
//             prevFreePtr = currFreePtr;
//             currFreePtr = currFreePtr->next;
//             break;
//         }
//         prevFreePtr = currFreePtr;
//         currFreePtr = currFreePtr->next;
//     }
//     if(currFreePtr->next == NULL) {
//         if (prevFreePtr == NULL) {
//             // only one free block
//             if(currFreePtr > startBlock) {
//                 startBlock->next = currFreePtr;
//                 freeHead = startBlock;
//                 mergeBlocks(freeHead, currFreePtr);
//             } else {
//                 currFreePtr->next = startBlock;
//                 mergeBlocks(currFreePtr, startBlock);
//             }
//         } else {
//             if (currFreePtr > startBlock) {
//                 //add to end of list
//                 currFreePtr->next = startBlock;
//                 mergeBlocks(currFreePtr, startBlock);
//             } else {
//                 // between last and second last
//                 prevFreePtr->next = startBlock;
//                 startBlock->next = currFreePtr;
//                 mergeBlocks(prevFreePtr, startBlock);
//                 mergeBlocks(startBlock, currFreePtr);
//             }
//             // currFreePtr->next = startBlock;
//             // mergeBlocks(currFreePtr, startBlock);
//         }
//     } else {
//         prevFreePtr->next = startBlock;
//         startBlock->next = currFreePtr;
//         mergeBlocks(prevFreePtr, startBlock);
//         mergeBlocks(startBlock, currFreePtr);
//     }
// }

void myfree(void* ptr) {
    if (!ptr) return;

    BLOCK* block = (BLOCK*)((char*)ptr - sizeof(BLOCK));
    block->next = NULL;

    // if free list empty
    if (!freeHead) {
        freeHead = block;
        return;
    }

    BLOCK* curr = freeHead;
    BLOCK* prev = NULL;

    // find sorted position
    while (curr && curr < block) {
        prev = curr;
        curr = curr->next;
    }

    // insert block
    block->next = curr;
    if (prev) {
        prev->next = block;
    } else {
        freeHead = block;
    }

    mergeBlocks(prev, block);
    mergeBlocks(block, curr);
}


// print free Linked list
void printLL() {
    BLOCK* start = freeHead;
    while(start != NULL) {
        printf("%p : %d\n", start, start->length);
        // if(start->next == start) break;
        start = start->next;
    }
}
void mergeBlocks(BLOCK* blockOne, BLOCK* blockTwo) {
    int isMergable = ((char*)blockOne + sizeof(BLOCK) + blockOne->length) == (char*)blockTwo;
    if(isMergable){
        blockOne->length = blockOne->length + blockTwo->length + sizeof(BLOCK);
        blockOne->next = blockTwo->next;
    }
}
