#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void assert(bool b) {
    static int testNumber = 0;
    testNumber++;
    if (! b) {
        printf("Test %d failed\n", testNumber);
        //Job Failed
        exit(1);
    }
}

void testNeighbours() {
    //CASE 1 (0, 0)

    //CASE 2 (4, 5)

    //CASE 3 (9, 9)

    //CASE 4 (0, 4)
}