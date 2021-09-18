#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
    if(argc != 2) {
        printf("Usage: ./nontiled <N>\n");
        return -1;
    }
    if(isdigit(atoi(argv[1])) != 0 || atoi(argv[1]) <= 0) {
        printf("Invalid N argument \"%s\"\n", argv[1]);
        return -1;
    }
    
    int N = atoi(argv[1]);
    double *arrayA = (double *)malloc(N * N * sizeof(double));
    double *arrayB = (double *)malloc(N * N * sizeof(double));
    int i, j;

    for(i = 0; i < N; i++) {
        for(j = 0; j < N; j++) {
            arrayA[N * i + j] = (double)(i + j);
            arrayB[N * i + j] = (double)(rand());
        }
    }

    return 0;
}
