#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>

struct timespec begin, end;
int main(int argc, char *argv[]) {
    if(argc != 3) {
        printf("Usage: ./tiled <array size> <tile size>\n");
        return -1;
    }
    int index;
    bool flagA = true;
    bool flagB = true;
    if(argv[1][0] == '-') {
 	    printf("Invalid N argument \"%s\"\n", argv[1]);
        flagA = false;
    } else {
        for(index=0; argv[1][index] != 0; index++) {
            if (!isdigit(argv[1][index])) {
                printf("Invalid N argument \"%s\"\n", argv[1]);
                flagA = false;
                break;
            }
        }
    }
    if(argv[2][0] == '-') {
 	    printf("Invalid B argument \"%s\"\n", argv[2]);
   	    flagB = false;
    } else {
        for(index=0; argv[2][index] != 0; index++) {
            if (!isdigit(argv[2][index])) {
                printf("Invalid B argument \"%s\"\n", argv[2]);
                flagB = false;
                break;
            }
        }
    }
    if(flagA == false || flagB == false) return -1;

    int N = atoi(argv[1]);
    int B = atoi(argv[2]);
    double *arrayA = (double *)malloc(N * N * sizeof(double));
    double *arrayB = (double *)malloc(N * N * sizeof(double));

    // initialize the Arrays
    int i, j, k, l;
    for(i = 0; i < N; i++) {
        for(j = 0; j < N; j++) {
            arrayA[N * i + j] = (double)(i + j);
            arrayB[N * i + j] = (double)(rand());
        }
    }

    // calculate
    clock_gettime(CLOCK_MONOTONIC, &begin);

    for(i=0; i<N; i+=(B>(N-i)? N%B : B)) {
        for(j=0; j<N; j+=(B>(N-j)? N%B: B)) {
            for(k=0; k<(B>(N-i)? N%B : B); k++) {
                for(l=0; l<(B>(N-j)? N%B : B); l++) {
                    arrayA[N*(i+k)+j+l] += arrayB[N*(j+l)+i+k];
                }
            }
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed_time =(1.0E+9 * (end.tv_sec - begin.tv_sec) + ( end.tv_nsec - begin.tv_nsec)) / 1.0E+9;
    printf("%lf\n", elapsed_time);
    return 0;
}
