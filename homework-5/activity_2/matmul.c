#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>
#include <string.h>
#include <math.h>

#define A(x,y) ARRAY_A[(x)*(TILESIZE)+(y)]
#define B(x,y) ARRAY_B[(x)*(TILESIZE)+(y)]

///////////////////////////
////// Main function //////
///////////////////////////

int main(int argc, char *argv[])
{
	int N;
	MPI_Init(&argc, &argv);

	if (argc < 2) {
		printf("Missing <Dimension of Matrix N> argument\n");
	} else {
		N = strtol(argv[1], NULL, 10);
	}

	// For testing purpose
	int buffer[1]; 
	// Determine rank and number of processes
	int num_procs;
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

	int TILESIZE =  (int)(N/(sqrt(num_procs)));
	int row_num = (int)(rank/sqrt(num_procs));
	int col_num = rank%(int)sqrt(num_procs);
	int num_of_columns = (int)sqrt(num_procs);
	// printf("rank [%d]\tComm_Row\tcolor [%d]\tkey[%d]\n", rank, row_num, col_num);
	// printf("rank [%d]\tComm_Row\tcolor [%d]\tkey[%d]\n", rank, col_num+(int)sqrt(num_procs), row_num);

	double *ARRAY_A = (double *)malloc(TILESIZE*TILESIZE*sizeof(double));
	double *ARRAY_B = (double *)malloc(TILESIZE*TILESIZE*sizeof(double));


	for(int i=0; i<TILESIZE; i++) {
		for(int j=0; j<TILESIZE; j++) {
			A(i, j) = i + row_num*TILESIZE;
			B(i, j) = i + row_num*TILESIZE + j+ col_num*TILESIZE;
		}
	}

	MPI_Comm new_comm_row;
	MPI_Comm new_comm_col;

	MPI_Comm_split(MPI_COMM_WORLD, row_num, col_num, &new_comm_row);
	MPI_Comm_split(MPI_COMM_WORLD, col_num+num_of_columns, row_num, &new_comm_col);

	if(rank == 0) buffer[0] = 1;
	if(rank == 1) buffer[0] = 2;
	if(rank == 2) buffer[0] = 3;

	MPI_Bcast(buffer, 1, MPI_INT, 0, new_comm_col);
	MPI_Bcast(buffer, 1, MPI_INT, 1, new_comm_col);
	MPI_Bcast(buffer, 1, MPI_INT, 2, new_comm_col);

	printf("rank [%d] buffer [%d]\n",rank, buffer[0]);
	
	// printf("rank %d has row comm with color [%d] and key [%d]\n", rank, row_num, col_num);
	// printf("rank %d has col comm with color [%d] and key [%d]\n", rank, col_num+num_of_columns, row_num);

	// test
/* 	if(rank==0) {
		buffer[0] = 43;
		MPI_Bcast(buffer, 1, MPI_INT, 0, MPI_COMM_WORLD);
	}
	// if(rank==1)	MPI_Recv(buffer, 1, MPI_BYTE, 0, 0, new_comm_row, MPI_STATUS_IGNORE);
	MPI_Barrier(MPI_COMM_WORLD);
	printf("rank: %d\t buffer [%d]\n", rank, buffer[0]); */
	
	

	
	MPI_Comm_free(&new_comm_row);
	MPI_Comm_free(&new_comm_col);

	MPI_Finalize();
	return 0;
}