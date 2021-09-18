#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/time.h>

struct timeval start, end;

int main(int argc, char *argv[]) {
    if(argc != 3) {
        printf("Usage: ./tiled <array size> <tile size>\n");
        return -1;
    }
    if(isdigit(atoi(argv[1])) != 0 || atoi(argv[1]) <= 0) {
        printf("Invalid N argument \"%s\"\n", argv[1]);
        return -1;
    }
    if(isdigit(isdigit(atoi(argv[2]))) != 0  || atoi(argv[2]) <= 0 ) {
        printf("Invalid B argument \"%s\"\n", argv[2]);
        return -1;
    }
    int N = atoi(argv[1]);
    int B = atoi(argv[2]);
    double *arrayA = (double *)malloc(N * N * sizeof(double));
    double *arrayB = (double *)malloc(N * N * sizeof(double));

    // initialize the Arrays
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            arrayA[N * i + j] = (double)(i + j);
            arrayB[N * i + j] = (double)(rand());
        }
    }

    // calculate
    gettimeofday(&start, NULL);
    for(int i=0; i<N; i+=B) {
        for(int j=0; j<N; j+=B) {
            for(int k=0; k<B; k++) {
                for(int l=0; l<B; l++) {
                    arrayA[(N*i+j) + (k*N) + l] += arrayB[(N*i+j) + (k*N) + l];
                }
            }
        }
    }
    gettimeofday(&end, NULL);
    
    double elapsed_time = (10.0E+6 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec) / 10.0E+6;
    printf("%lf\n", elapsed_time);
    return 0;
}
