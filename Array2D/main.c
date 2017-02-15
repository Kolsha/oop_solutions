#include <stdio.h>
#include <assert.h>
#include "array2d.h"

void run_tests(){
    // add tests fail way
    Array2D *arr = array_create(20, 10);
    assert(arr != NULL);

    array_fill(arr, 1);
    assert(array_get(arr, 0, 0) == 1);

    assert(array_get(NULL, 0, 0) == 0);

    assert(array_get(arr, -1, -1) == 0);

    array_set(arr, 1, 1, 999);
    assert(array_get(arr, 1, 1) == 999);

    array_set(NULL, 1, 1, 777);
    assert(array_get(arr, 1, 1) == 999);

    array_delete(arr);

    printf("All tests passed!\n");
}


int main(void)
{
    printf("Hello World!\n");
    run_tests();
    return 0;
}

