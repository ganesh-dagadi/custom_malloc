#include <stdio.h>
#include "myalloc.h"
int main() {
    test1_freeBlockReused();
    test2_mergingThreeFreeBlocks();
    test3_mergingThreeFreeBlocksAndSpliyDuringAlloc();
    test4_heapNoLeak();
    return 0;
}

int test1_freeBlockReused() {
    int testPass = 1;
    int* p1 = (int*)myalloc(sizeof(int));
    *p1 = 10;
    testPass = testPass & (*p1 == 10);
    int* p2 = (int*)myalloc(sizeof(int));
    *p2 = 20;
    testPass = testPass & (*p2 == 20);
    int* p3 = (int*)myalloc(sizeof(int));
    *p3 = 30;
    testPass = testPass & (*p3 == 30);
    int* p4 = (int*)myalloc(sizeof(int));
    *p4 = 40;
    testPass = testPass & (*p4 == 40);
    myfree(p2);
    int* p5 = (int*)myalloc(sizeof(int));
    *p5 = 50;
    testPass = testPass & (p5 == p2);
    testPass = testPass & (*p5 == 50);
    if (testPass) {
        printf("Test 1: Pass \n");
    } else {
        printf("Test 1 Fail\n");
    }
    myfree(p1);
    myfree(p3);
    myfree(p4);
    myfree(p5);
}

int test2_mergingThreeFreeBlocks() {
    int testPass = 1;
    int* p1 = (int*)myalloc(sizeof(int));
    *p1 = 10;
    testPass = testPass & (*p1 == 10);
    int* p2 = (int*)myalloc(sizeof(int));
    *p2 = 20;
    testPass = testPass & (*p2 == 20);
    int* p3 = (int*)myalloc(sizeof(int));
    *p3 = 30;
    testPass = testPass & (*p3 == 30);
    int* p4 = (int*)myalloc(sizeof(int));
    *p4 = 40;
    testPass = testPass & (*p4 == 40);
    myfree(p2);
    myfree(p3);
    myfree(p4);
    long* p5 = (long*)myalloc(sizeof(long));
    
    *p5 = 50;
    testPass = testPass & (p5 == p2);
    testPass = testPass & (*p5 == 50);
    if (testPass) {
        printf("Test 2: Pass \n");
    } else {
        printf("Test 2: Fail\n");
    }
}

int test3_mergingThreeFreeBlocksAndSpliyDuringAlloc() {
    int testPass = 1;
    int* p1 = (int*)myalloc(8);
    *p1 = 10;
    testPass = testPass & (*p1 == 10);
    int* p2 = (int*)myalloc(8);
    *p2 = 20;
    testPass = testPass & (*p2 == 20);
    int* p3 = (int*)myalloc(8);
    *p3 = 30;
    testPass = testPass & (*p3 == 30);
    int* p4 = (int*)myalloc(8);
    *p4 = 40;
    testPass = testPass & (*p4 == 40);
    int* pTemp = (int*)myalloc(8);
    myfree(p3);
    myfree(p4);
    myfree(pTemp);
    long* p5 = (long*)myalloc(16);
    *p5 = 5000000000000;
    testPass = testPass & (p5 == p3);
    testPass = testPass & (*p5 == 5000000000000);
    int* p6 = (int*)myalloc(8);
    *p6 = 60;
    testPass = testPass & (*p6 == 60);
    testPass = testPass & (p6 == ((char*)pTemp - sizeof(BLOCK)));
    if (testPass) {
        printf("Test 3: Pass \n");
    } else {
        printf("Test 3: Fail\n");
    }
}

int test4_heapNoLeak() {
    void* blockSmall = myalloc(128);
    void* blockSmall2 = myalloc(128);
    void* blockSmall3 = myalloc(128);
    void* blockSmall4= myalloc(128);
    void* heapEndSmall = heapEnd;
    myfree(blockSmall);
    myfree(blockSmall2);
    myfree(blockSmall3);
    myfree(blockSmall4);
    void* bigBlock = myalloc(584);
    int testPass = heapEndSmall == heapEnd;
    printf("%p, %p", heapEnd, heapEndSmall);
    if (testPass) {
        printf("Test 4: Pass \n");
    } else {
        printf("Test 4: Fail\n");
    }
}