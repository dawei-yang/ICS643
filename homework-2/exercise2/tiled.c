#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/time.h>
#include <stdbool.h>

struct timeval start, end;

int main(int argc, char *argv[]) {
    if(argc != 3) {
        printf("Usage: ./tiled <array size> <tile size>\n");
        return -1;
    }
    int p;
    bool flagA = true;
    bool flagB = true;
    if(argv[1][0] == '-') {
 	    printf("Invalid N argument \"%s\"\n", argv[1]);
        flagA = false;
    } else {
        for(p=0; argv[1][p] != 0; p++) {
            if (!isdigit(argv[1][p])) {
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
        for(p=0; argv[2][p] != 0; p++) {
            if (!isdigit(argv[2][p])) {
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
    gettimeofday(&start, NULL);
    for(i=0; i<N; i+=B) {
        for(j=0; j<N; j+=B) {
            for(k=0; k<B; k++) {
                for(l=0; l<B; l++) {
                    arrayA[(N*i+j) + (k*N) + l] += arrayB[(N*j+i) + (l*N) + k];
                }
            }
        }
    }
    gettimeofday(&end, NULL);

    double elapsed_time = (10.0E+6 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec) / 10.0E+6;
    printf("%lf\n", elapsed_time);
    return 0;
}
