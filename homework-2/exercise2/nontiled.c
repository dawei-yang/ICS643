#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void printArray(int rowSize, int colSize, int array[rowSize][rowSize]);

int main(int argc, char *argv[]) {
    if(argc != 2) {
        printf("Please enter the size of the array N\n");
        return -1;
    }
    if(isdigit(atoi(argv[1])) != 0 || atoi(argv[1]) <= 0) {
        printf("Invalid N argument \"%s\"\n", argv[1]);
        return -1;
    }
/*     if(atoi(argv[1]) <= 0) {
        printf("Invalid N argument \"%s\"\n", argv[1]);
        return -1;
    } */

/*     int rowSize = atoi(argv[1]);
    int colSize = atoi(argv[1]);
    int arrayA[rowSize][colSize]; */

    int N = atoi(argv[1]);
    double *arrayA = (double *)malloc(N * N * sizeof(double));
    double *arrayB = (double *)malloc(N * N * sizeof(double));

    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            arrayA[N * i + j] = (double)(i + j);
            arrayB[N * i + j] = (double)(rand());
        }
    }

    printf("Array A: \n");
    for(int k = 0; k < N*N; k++) {
        printf("%lf ", arrayA[k]);
    }
    printf("\nArray B: \n");
    for(int k = 0; k < N*N; k++) {
        printf("%lf ", arrayB[k]);
    }
    printf("\n");

/*     for(int i = 0; i < 5; i++) {
        for(int j = 0; j < 5; j++) {
            arrayA[i][j] = i + j;
        }
    }
    printArray(rowSize, colSize, arrayA); */
    return 0;
}

void printArray(int rowSize, int colSize, int array[rowSize][rowSize]) {
    for(int i = 0; i < rowSize; i++) {
        for(int j = 0; j < colSize; j++) {
            printf("%d\t", array[i][j]);
        }
        printf("\n");
    }
}