#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>
#include <string.h>
#include <math.h>

#define A(x,y) ARRAY_A[(x)*(TILESIZE)+(y)]
#define B(x,y) ARRAY_B[(x)*(TILESIZE)+(y)]

static void program_abort(char *message) {
  int my_rank;
  MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
  if (my_rank == 0) {
    if (message) {
      fprintf(stderr,"%s",message);
    }
  }
  MPI_Abort(MPI_COMM_WORLD, 1);
  exit(1);
}

static int isPerfective(int number) {
	int iVar;
    float fVar;
    fVar=sqrt((double)number);
    iVar=fVar;
    if(iVar==fVar) return 1;
    else return 0;
}


int main(int argc, char *argv[])
{
	int N, i, j, k, q, num_procs, rank;
	MPI_Init(&argc, &argv);

	if(argc < 2) {
		program_abort("Missing Dimension of Matrix <N>\n");
	}

	N = strtol(argv[1], NULL, 10);
	if(N <= 0) {
        program_abort("Invalid argument <N>\n");
 
    }	
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

	if(isPerfective(num_procs) == 0) program_abort("Number of processes is not a perfect number\n");
	int p = (int)sqrt(num_procs);
	if(N%p !=0) program_abort("square root of number of process does not divide N\n");


	int TILESIZE =  (int)(N/(sqrt(num_procs)));
	int row_num = (int)(rank/sqrt(num_procs));
	int col_num = rank%(int)sqrt(num_procs);

	double *ARRAY_A = (double *)malloc(TILESIZE*TILESIZE*sizeof(double));
	double *ARRAY_B = (double *)malloc(TILESIZE*TILESIZE*sizeof(double));


	for(i=0; i<TILESIZE; i++) {
		for(j=0; j<TILESIZE; j++) {
			A(i, j) = i + row_num*TILESIZE;
			B(i, j) = i + row_num*TILESIZE + j+ col_num*TILESIZE;
		}
	}
	printf("Process Rank %d (coordinates: %d, %d), block of A: \n", rank, row_num, col_num);
	for(i=0; i<TILESIZE; i++) {
		for(j=0; j<TILESIZE; j++) {
			printf("%.2lf\t", A(i, j));
		}
		printf("\n");
	}
	printf("Process Rank %d (coordinates: %d, %d), block of B: \n", rank, row_num, col_num);
	for(i=0; i<TILESIZE; i++) {
		for(j=0; j<TILESIZE; j++) {
			printf("%.2lf\t", B(i, j));
		}
		printf("\n");
	}

	MPI_Finalize();
	return 0;
}