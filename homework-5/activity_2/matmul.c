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

static int isPerfectSquare(int number) {
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

	if(isPerfectSquare(num_procs) == 0) program_abort("Number of processes is not a perfect number\n");
	int p = (int)sqrt(num_procs);
	if(N%p !=0) program_abort("square root of number of process does not divide N\n");

	int TILESIZE = N/p;
	int row_num = rank/p;
	int col_num = rank%p;
	double send_data = 0.0;
	double recv_data = 0.0;

	// Allocate Memory for Arrays
	double *ARRAY_A = (double *)malloc(TILESIZE*TILESIZE*sizeof(double));
	double *ARRAY_B = (double *)malloc(TILESIZE*TILESIZE*sizeof(double));
	double *ARRAY_C = (double *)malloc(TILESIZE*TILESIZE*sizeof(double));
	double *TEMP_A, *TEMP_B;

	// Initialize A, B, C
	for(i=0; i<TILESIZE; i++) {
		for(j=0; j<TILESIZE; j++) {
			A(i, j) = i+row_num*TILESIZE;
			B(i, j) = i+row_num*TILESIZE+j+col_num*TILESIZE;
			C(i, j) = 0;
		}
	}

	// Establish row and col sub communicators
	MPI_Comm new_comm_row;
	MPI_Comm new_comm_col;
	MPI_Comm_split(MPI_COMM_WORLD, row_num, col_num, &new_comm_row);
	MPI_Comm_split(MPI_COMM_WORLD, col_num+p, row_num, &new_comm_col);


	for(k=0; k<=p-1; k++) {
		// Set temporary pointers
		TEMP_A = ARRAY_A;
		TEMP_B = ARRAY_B;

		// Boardcast A_?k OR B_k?
		MPI_Bcast(&TEMP_A, 2, MPI_INT, k, new_comm_row);
		MPI_Bcast(&TEMP_B, 2, MPI_INT, k, new_comm_col);
		MPI_Barrier(MPI_COMM_WORLD);

		// Calculate C
		for(i=0; i<TILESIZE; i++) {
			for(j=0; j<TILESIZE; j++) {	
				for(q=0; q<TILESIZE; q++) {
					C(i, j) += TA(i, q)*TB(q, j);
					printf("rank [%d], C[%d] = %lf\n", rank, i*TILESIZE+j, ARRAY_C[i * TILESIZE + j]);
				}
			}
		}
	}

	// calculate sum of C for each process
	for(i=0; i<TILESIZE; i++) {
		for(j=0; j<TILESIZE; j++) {
			send_data += C(i, j);
		}
	}

	MPI_Reduce(&send_data, &recv_data, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	// Result validation in rank 0
	if(rank == 0) {
		double expected_sum = (pow(N, 3)*pow(N-1, 2))/2;
		printf("tilesize = %d\n", TILESIZE);
		fprintf(stderr, "Expected sum of C: \t%lf\n", expected_sum);
		fprintf(stderr, "Calculate sum of C: \t%lf\n", recv_data);
		if(abs(expected_sum-recv_data) < 0.000000001) fprintf(stderr,"Multiplication Correct\n");
		else fprintf(stderr,"Multiplication Incorrect\n");
	}

	MPI_Comm_free(&new_comm_row);
	MPI_Comm_free(&new_comm_col);
	MPI_Finalize();
	return 0;
}