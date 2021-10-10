#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>

int main(int argc, char **argv) {

    // Parse comand-line arguments
    if (argc != 3) {
        fprintf(stderr,"Usage: %s <array size> <number of threads>\n", argv[0]);
        exit(1);
    }

    int N = strtol(argv[1], NULL, 10);
    int T = strtol(argv[2], NULL, 10);
    if (N <= 0 || T <=0) {
        fprintf(stderr,"Invalid arguments\n");
        exit(1);
    }

    // Allocate and initialize array
    fprintf(stderr,"Array allocation and initialization...\n");
    int *A = (int *)malloc(N * N * sizeof(int));
    int *B = (int *)malloc(N * N * sizeof(int));
    int *C = (int *)calloc(N * N, sizeof(int));
    int i,j,k;
    for (int i=0; i < N*N; i++) {
        A[i] = rand() % 1024;
        B[i] = rand() % 1024;
    }

    // Matrix multiplication
    fprintf(stderr, "Matrix multiplication...\n");
    fprintf(stderr, "Number of Threads = %d\n", T);
    struct timeval begin, end;
    omp_set_num_threads(T);
    gettimeofday(&begin, NULL);

	#ifdef PARALLEL_I
	#pragma omp parallel for private(i, j, k)
    for (i=0; i < N; i++) {
        for (k=0; k < N; k++) {
            for (j=0; j < N; j++) {
                C[i*N + j] += A[i*N + k] * B[k*N + j];
            }
        }
    }
	#endif

	#ifdef PARALLEL_K
    for (i=0; i < N; i++) {
    #pragma omp parallel for private(k, j)
        for (k=0; k < N; k++) {
            for (j=0; j < N; j++) {
                // printf("C[%d] = C[%d] + A[%d] + B[%d]\n", i*N+j, i*N+j, i*N+k, k*N+j);
                #pragma omp atomic
                C[i*N + j] +=  A[i*N + k] * B[k*N + j];
                
            }
        }
    }
	#endif

    #ifdef PARALLEL_J
    for (i=0; i < N; i++) {
        for (k=0; k < N; k++) {
            #pragma omp parallel for private(j)
            for (j=0; j < N; j++) {
                C[i*N + j] += A[i*N + k] * B[k*N + j];
            }
        }
    }
	#endif  
    gettimeofday(&end, NULL);

    /* Check Parallel Results by comparing each element with the sequential one*/
    #ifdef CHECK
    int *D = (int *)calloc(N * N, sizeof(int));
    for (i=0; i < N; i++) {
      for (k=0; k < N; k++) {
        for (j=0; j < N; j++) {
          D[i*N + j] += A[i*N + k] * B[k*N + j];
        }
      }
    }

    int same = 1;
    int p;
    for (p=1; p < N*N; p++) {
        if(D[p] != C[p]) {
            same = 0;
            break;
        }
    }
    if(same == 1) fprintf(stderr, "Element-To-Element Check Passed\n");
    else fprintf(stderr, "Element-To-Element Check Failed\n");
    #endif

    double elapsed = (1.0E+6 * (end.tv_sec - begin.tv_sec) + end.tv_usec -  begin.tv_usec) / 1.0E+6;
    fprintf(stdout, "%.2lf\n", elapsed);

}