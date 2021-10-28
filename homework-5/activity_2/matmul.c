#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>
#include <string.h>
#include <math.h>

#define A(x,y) ARRAY_A[(x)*(TILESIZE)+(y)]
#define B(x,y) ARRAY_B[(x)*(TILESIZE)+(y)]
#define C(x,y) ARRAY_C[(x)*(TILESIZE)+(y)]
#define TA(x,y) TEMP_A[(x)*(TILESIZE)+(y)]
#define TB(x,y) TEMP_B[(x)*(TILESIZE)+(y)]


int main(int argc, char *argv[])
{
	int N;
	MPI_Init(&argc, &argv);

	if (argc < 2) {
		printf("Missing <Dimension of Matrix N> argument\n");
	} else {
		N = strtol(argv[1], NULL, 10);
	}

	// Determine rank and number of processes
	int num_procs;
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

	int p = (int)sqrt(num_procs);
	int TILESIZE =  (int)(N/p);
	int row_num = (int)(rank/p);
	int col_num = rank%p;

	double *ARRAY_A = (double *)malloc(TILESIZE*TILESIZE*sizeof(double));
	double *ARRAY_B = (double *)malloc(TILESIZE*TILESIZE*sizeof(double));
	double *ARRAY_C = (double *)malloc(TILESIZE*TILESIZE*sizeof(double));
	double *TEMP_A = (double *)malloc(TILESIZE*TILESIZE*sizeof(double));
	double *TEMP_B = (double *)malloc(TILESIZE*TILESIZE*sizeof(double));

	// initialize
	for(int i=0; i<TILESIZE; i++) {
		for(int j=0; j<TILESIZE; j++) {
			A(i, j) = i+row_num*TILESIZE;
			B(i, j) = i+row_num*TILESIZE+j+col_num*TILESIZE;
		}
	}

	// Establish row and col sub communicators
	MPI_Comm new_comm_row;
	MPI_Comm new_comm_col;
	MPI_Comm_split(MPI_COMM_WORLD, row_num, col_num, &new_comm_row);
	MPI_Comm_split(MPI_COMM_WORLD, col_num+p, row_num, &new_comm_col);

	for(int k = 0; k<=p-1; k++) {
		// copy to temp 
		for(int i=0; i<TILESIZE; i++) {
			for(int j=0; j<TILESIZE; j++) {
				TA(i, j) = A(i, j);
				TB(i, j) = B(i, j);
			}
		}

		// Boardcast A_?k OR B_k?
		MPI_Bcast(TEMP_A, TILESIZE*TILESIZE, MPI_DOUBLE, k, new_comm_row);
		MPI_Bcast(TEMP_B, TILESIZE*TILESIZE, MPI_DOUBLE, k, new_comm_col);
		MPI_Barrier(MPI_COMM_WORLD);

		// calculate C
		for(int i=0; i<TILESIZE; i++) {
			for(int j=0; j<TILESIZE; j++) {
				// printf("rank [%d]\t TA = %lf; TB = %lf\n", rank, TA(i, j), TB(i, j));
				C(i, j) += TA(i, j)*TB(i, j);
			}
		}
	}

	// calculate sum of C for evaluation
	double sum = 0;
	for(int i=0; i<TILESIZE; i++) {
		for(int j=0; j<TILESIZE; j++) {
			sum += C(i, j);
		}
	}
	printf("rank [%d] sum = %lf\n", rank, sum);

	MPI_Comm_free(&new_comm_row);
	MPI_Comm_free(&new_comm_col);
	MPI_Finalize();
	return 0;
}