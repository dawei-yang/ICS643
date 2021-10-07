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

    double *arrayC = (double *)malloc(N * N * sizeof(double));
    double *arrayD = (double *)malloc(N * N * sizeof(double));

    // initialize the Arrays
    int i, j, k, l, p, q;
    for(i = 0; i < N; i++) {
        for(j = 0; j < N; j++) {
            arrayA[N * i + j] = (double)(i + j);
            arrayB[N * i + j] = (double)(rand());
        }
    }

    for(i = 0; i < N; i++) {
        for(j = 0; j < N; j++) {
            //printf("-> [%d, %d]\n", (N*i+j), (N*j+i));
            arrayC[N * i + j] = arrayA[N * i + j] + arrayB[N * j + i];
        }
    }

    // calculate
    gettimeofday(&start, NULL);
/*     for (i=0; i< N; i+=B) {
        for (j=0; j < N; j+=B) {
            for (p=i; p < i+B; p++) {
                for (q=j; q < j+B; q++) {
                    printf("A[%d] + B[%d]\n", B*p+q, B*q+p);
                    arrayD[B * p + q] = arrayA[B * p + q] + arrayB[B * q + p];
                }
            }
        }
    } */


    for(i=0; i<N; i+=(B>(N-i)? N%B : B)) {
        for(j=0; j<N; j+=(B>(N-j)? N%B: B)) {
            for(k=0; k<(B>(N-i)? N%B : B); k++) {
                for(l=0; l<(B>(N-j)? N%B : B); l++) {
                    printf("A[%d] + B[%d]\n", (N*(i+k)+j+l), (N*(j+l)+i+k));
                    // arrayA[(N*i+j) + (k*N) + l] += arrayB[(N*j+i) + (l*N) + k];
                    arrayD[N*(i+k)+j+l] = arrayA[N*(i+k)+j+l] += arrayB[N*(j+l)+i+k];

                }
            }
        }
    }
    gettimeofday(&end, NULL);
    double sumC = 0.0;
    double sumD = 0.0;
    for(i=0; i<N*N; i++) {
        sumC += arrayC[i];
        sumD += arrayD[i];
    }

    /* for(i = 0; i < N*N; i++) {
        printf("%lf\t", arrayC[i]);
    }
    printf("\n");
    for(i = 0; i < N*N; i++) {
        printf("%lf\t", arrayD[i]);
    } */

    double elapsed_time = (10.0E+6 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec) / 10.0E+6;
    printf("%lf\n", elapsed_time);
    printf("arrayC: %lf\n", sumC);
    printf("arrayD: %lf\n", sumD);
    return 0;
}
