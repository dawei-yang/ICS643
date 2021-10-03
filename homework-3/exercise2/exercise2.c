#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>

/* Convenient macro to access array element */
#define ARRAY(x,y) A[(x)*(N+2)+(y)]
/* Convenient macro to compute array element update */
#define UPDATE(x,y) ((ARRAY(x,y) + ARRAY(x-1,y) + ARRAY(x,y-1))/3.0);

int main(int argc, char **argv) {

  // Parse command-line arguments
  if (argc != 3) {
    fprintf(stderr,"Usage: %s <array size> <num iterations>\n", argv[0]);
    exit(1);
  }

  int N = strtol(argv[1], NULL, 10);
  int num_iterations = strtol(argv[2], NULL, 10);

  if (N <= 0 || num_iterations <=0) {
    fprintf(stderr,"Invalid arguments\n");
    exit(1);
  }

  // Allocate (N+2)x(N+2) array
  double *A = (double *)malloc((N+2) * (N+2) * sizeof(double));

  // Array initialization
  fprintf(stderr,"Array initialization...\n");
  srand(42);
  for (int i=0; i < N+2; i++) {
    for (int j=0; j < N+2; j++) {
      ARRAY(i,j) = (double)rand();
    }
  }

  // Loop for num_iterations iterations
  fprintf(stderr,"Computing all iterations...\n");
  struct timeval begin, end;
  gettimeofday(&begin, NULL);

  for (int iter = 0; iter < num_iterations; iter++) {

    int i,j;
    for (i = 1; i < N+1; i++) {
      for (j = 1; j < N+1; j++) {
        ARRAY(i,j) = UPDATE(i,j);
        // printf("[%d, %d] <= [%d, %d] + [%d, %d] + [%d, %d]\n", i, j, i, j, i-1, j, i, j-1);
      }
    }

  }

  gettimeofday(&end, NULL);

  double elapsed = (1.0E+6 * (end.tv_sec - begin.tv_sec) + end.tv_usec -  begin.tv_usec) / 1.0E+6;
  fprintf(stdout, "time: %.2lf\n", elapsed);

  // Compute and print the sum of elements for 
  // correctness checking (may overflow, whatever)
  double checksum =0;
  for (int i=1; i < N+1; i++) {
    for (int j=1; j < N+1; j++) {
      checksum += ARRAY(i,j);
    }
  }
  fprintf(stdout,"checksum: %.10f\n",checksum);

  exit(0);
}