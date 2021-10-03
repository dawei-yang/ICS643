#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <omp.h>


/* Convenient macro to access array element */
#define ARRAY(x,y) A[(x)*(N+2)+(y)]
/* Convenient macro to compute array element update */
#define UPDATE(x,y) ((ARRAY(x,y) + ARRAY(x-1,y) + ARRAY(x,y-1))/3.0);

/* Array B for -DCHECK */
#ifdef CHECK
#define ARRAYB(x,y) B[(x)*(N+2)+(y)]
#define UPDATEB(x,y) ((ARRAYB(x,y) + ARRAYB(x-1,y) + ARRAYB(x,y-1))/3.0);
#endif



int main(int argc, char **argv) {

  // Parse command-line arguments
  if (argc != 4) {
    fprintf(stderr,"Usage: %s <array size> <num iterations> <num threads>\n", argv[0]);
    exit(1);
  }

  int N = strtol(argv[1], NULL, 10);
  int num_iterations = strtol(argv[2], NULL, 10);
  int num_threads = strtol(argv[3], NULL, 10);

  if (N <= 0 || num_iterations <=0 || num_threads <=0) {
    fprintf(stderr,"Invalid arguments\n");
    exit(1);
  }

  // Allocate (N+2)x(N+2) array
  double *A = (double *)malloc((N+2) * (N+2) * sizeof(double));

  #ifdef CHECK
  double *B = (double *)malloc((N+2) * (N+2) * sizeof(double));
  #endif
  // Array initialization
  fprintf(stderr,"Array initialization...\n");
  srand(42);
  for (int i=0; i < N+2; i++) {
    for (int j=0; j < N+2; j++) {
      double r = (double)rand();
      ARRAY(i, j) = r;
      #ifdef CHECK
      ARRAYB(i, j) = r;
      #endif
    }
  }

  // Loop for num_iterations iterations
  fprintf(stderr,"Computing all iterations...\n");
  omp_set_num_threads(num_threads);
  struct timeval begin, end;
  gettimeofday(&begin, NULL);
  
  int size_of_tile = 200;
  for (int iter = 0; iter < num_iterations; iter++) {
    for(int k = 1 ; k < N * 2 ; k+=size_of_tile) {
      // Parallel for tiles anti-diagonal
      #pragma omp parallel for
        for( int j = 1 ; j <= k ; j+=size_of_tile) {
          int i = k - j + 1;
          if( i < N && j < N ) {
            // Parallel each tiles anti-diagonal
            int start_i = i;
            int start_j = j;
            int end_i = i+size_of_tile -1;
            int end_j = j+size_of_tile -1;
            for(int j = start_j; j <= end_j; j++) {
              #pragma omp parallel for
              for(int m = j; m >= start_j; m--) {
                for(int n=(start_i+j-m); n<(start_i+j+1-m); n++) {
                  ARRAY(n, m) = UPDATE(n, m);
                }
              }
            }
            for(int j = start_j+1; j <= end_j; j++) {
              #pragma omp parallel for
              for(int m = j; m<=end_j; m++) {
                for(int n= (end_i+j-m); n<(end_i+j-m+1); n++) {
                  ARRAY(n, m) = UPDATE(n, m);
                }
              }
            }
          }
        }
    }
  }
  gettimeofday(&end, NULL);


  #ifdef CHECK
  for (int iter = 0; iter < num_iterations; iter++) {
    for (int p = 1; p < N+1; p++) {
      for (int q = 1; q < N+1; q++) {
          ARRAYB(p, q) = UPDATEB(p, q);
      }
    }
  }
  #endif  

  double elapsed = (1.0E+6 * (end.tv_sec - begin.tv_sec) + end.tv_usec -  begin.tv_usec) / 1.0E+6;
  fprintf(stdout, "time: %.2lf\n", elapsed);

  // Compute and print the sum of elements for 
  // correctness checking (may overflow, whatever)
  #ifdef CHECK
  double checksum =0;
  double checksum_reg = 0;

  for (int p=1; p < N+1; p++) {
    for (int q=1; q < N+1; q++) {
      checksum_reg += ARRAY(p, q);
      checksum += ARRAYB(p, q);
    }
  }
  fprintf(stdout, "Sequential Checksum: %.10f\n",checksum);
  fprintf(stdout, "Parallel Checksum:   %.10f\n", checksum_reg);
  #endif

  exit(0);
}
