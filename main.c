#include <stdio.h>
#include "myalloc.h"
int main() {
    // test1_freeBlockReused();
    // test2_mergingThreeFreeBlocks();
    test3_mergingThreeFreeBlocksAndSpliyDuringAlloc();
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
    int* p1 = (int*)myalloc(128);
    *p1 = 10;
    testPass = testPass & (*p1 == 10);
    printf("%p: %d \n", p1, *p1);
    int* p2 = (int*)myalloc(128);
    *p2 = 20;
    printf("%p: %d \n", p2, *p2);
    testPass = testPass & (*p2 == 20);
    int* p3 = (int*)myalloc(128);
    *p3 = 30;
    printf("%p: %d \n", p3, *p3);
    testPass = testPass & (*p3 == 30);
    int* p4 = (int*)myalloc(128);
    *p4 = 40;
    printf("%p: %d \n", p4, *p4);
    testPass = testPass & (*p4 == 40);
    int* pTemp = (int*)myalloc(128);
    printf("%p: %d \n", pTemp, *pTemp);
    myfree(p3);
    myfree(p4);
    myfree(pTemp);
    long* p5 = (long*)myalloc(256);
    *p5 = 5000000000000;
    printf("%p: %ld \n", p5, *p5);
    testPass = testPass & (p5 == p3);
    testPass = testPass & (*p5 == 5000000000000);
    int* p6 = (int*)myalloc(128);
    *p6 = 60;
    printf("%p: %ld \n", p6, *p6);
    testPass = testPass & (*p6 == 60);
    testPass = testPass & (p6 == pTemp);
    if (testPass) {
        printf("Test 3: Pass \n");
    } else {
        printf("Test 3: Fail\n");
    }
}