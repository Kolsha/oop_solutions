#include <stdlib.h>

#include "array2d.h"


Array2D *array_create(int m, int n){

    if(m < 1 || n < 1){
        return NULL;
    }

    Array2D *res = (Array2D*) malloc(sizeof(Array2D));
    if(res == NULL){
        return NULL;
    }

    res->arr = (int*) malloc(sizeof(int) * m * n);
    if(res->arr == NULL){
        free(res);
        return NULL;
    }

    res->height = m;
    res->width = n;

    return res;
}

void array_delete(Array2D *arr){
    if(arr == NULL){
        return ;
    }
    free(arr->arr);
    free(arr);
}

static int check_access(Array2D *arr, int i, int j){

    if(i < 0 || j < 0){
        return 0;
    }

    if(arr == NULL || arr->arr == NULL){
        return 0;
    }

    if(i >= arr->height || j >= arr->width){
        return 0;
    }

    return 1;
}

int array_get(Array2D *arr, int i, int j){

    if(!check_access(arr, i, j)){
        return 0;
    }

    return arr->arr[i + arr->height*j];
}

void array_set(Array2D *arr, int i, int j, int value){

    if(!check_access(arr, i, j)){
        return ;
    }

    arr->arr[i + arr->height*j] = value;
}

void array_fill(Array2D *arr, int value){

    if(arr == NULL ||  arr->arr == NULL){
        return ;
    }

    for(int i = 0; i < arr->height; i++){
        for(int j = 0; j < arr->width; j++){
            array_set(arr, i, j, value) ;
        }
    }
}
