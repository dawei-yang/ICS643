#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <string.h>
#include <math.h>

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

int main(int argc, char *argv[])
{
    int N;
    MPI_Init(&argc, &argv);
    if(argc < 2) {
		program_abort("Missing Dimension of Matrix <N>\n");
	}

	N = strtol(argv[1], NULL, 10);
	if(N <= 0) {
        program_abort("Invalid argument <N>\n");
 
    }	
    MPI_Comm cannon_comm;
    MPI_Status status_A1, status_A2, status_B;
    MPI_Request request_A1, request_A2;
    int rank, size, shift, TILESIZE, i, j, k;
    int dims[2];
    int periods[2];
    int left, right, up, down;
    double *A, *B, *C;
    double *buf, *tmp;
    double start, end;
    double send_data = 0.0;
	double recv_data = 0.0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    dims[0] = 0;
    dims[1] = 0;
    periods[0] = 1;
    periods[1] = 1;

    //create dims use dims to check if it is perfect square
    MPI_Dims_create(size, 2, dims);
    if (dims[0] != dims[1])
    {
        if (rank == 0)
            printf("The number of processors must be a square.\n");
        MPI_Finalize();
        return 0;
    }
    TILESIZE = N / dims[0];

	int row_num = (int)(rank/dims[0]);
	int col_num = rank%(dims[0]);
    A = (double *)malloc(TILESIZE * TILESIZE * sizeof(double));
    B = (double *)malloc(TILESIZE * TILESIZE * sizeof(double));
    buf = (double *)malloc(TILESIZE * TILESIZE * sizeof(double));

    C = (double *)calloc(TILESIZE * TILESIZE, sizeof(double));
    for (i = 0; i < TILESIZE; i++) {
        for (j = 0; j < TILESIZE; j++){
            A[i * TILESIZE + j] = i + row_num * TILESIZE;
            B[i * TILESIZE + j] = i + row_num * TILESIZE + j + col_num * TILESIZE;
            C[i * TILESIZE + j] = 0.0;
        }
    }

	/* printf("Process Rank %d (coordinates: %d, %d), block of A: \n", rank, row_num, col_num);
	for(i=0; i<TILESIZE; i++) {
		for(j=0; j<TILESIZE; j++) {
			printf("%.2lf\t", A[i*TILESIZE + j]);
		}
		printf("\n");
	}
	printf("Process Rank %d (coordinates: %d, %d), block of B: \n", rank, row_num, col_num);
	for(i=0; i<TILESIZE; i++) {
		for(j=0; j<TILESIZE; j++) {
			printf("%.2lf\t", B[i * TILESIZE + j]);
		}
		printf("\n");
	} */


    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 1, &cannon_comm);
    MPI_Cart_shift(cannon_comm, 0, 1, &left, &right);
    MPI_Cart_shift(cannon_comm, 1, 1, &up, &down);

    start = MPI_Wtime();
    for (shift = 0; shift < dims[0]; shift++)
    {
        // Matrix multiplication
        for (i = 0; i < TILESIZE; i++)
            for (k = 0; k < TILESIZE; k++)
                for (j = 0; j < TILESIZE; j++) {
                    C[i * TILESIZE + j] += A[i * TILESIZE + k] * B[k * TILESIZE + j];
                    // printf("rank [%d], C[%d] = %lf\n", rank, i*TILESIZE+j, C[i * TILESIZE + j]);
                }
        
        // Communication
        MPI_Isend(A, TILESIZE*TILESIZE, MPI_DOUBLE, left, 1, cannon_comm, &request_A1);
        MPI_Send(B, TILESIZE*TILESIZE, MPI_DOUBLE, up, 2, cannon_comm);
        MPI_Irecv(A, TILESIZE*TILESIZE, MPI_DOUBLE, right, 1, cannon_comm, &request_A2);    
        MPI_Recv(B, TILESIZE*TILESIZE, MPI_DOUBLE, down, 2, cannon_comm, &status_B);
        MPI_Wait(&request_A1, &status_A1);
        MPI_Wait(&request_A2, &status_A2);


        // Communication
        /* MPI_Send(A, TILESIZE*TILESIZE, MPI_DOUBLE, left, 1, cannon_comm);
        MPI_Recv(buf, TILESIZE*TILESIZE, MPI_DOUBLE, right, 1, cannon_comm, &status);
    
        tmp = buf;
        buf = A;
        A = tmp; 
        MPI_Send(B, TILESIZE*TILESIZE, MPI_DOUBLE, up, 2, cannon_comm);
        MPI_Recv(buf, TILESIZE*TILESIZE, MPI_DOUBLE, down, 2, cannon_comm, &status);
        B = buf;
        tmp = buf;
        buf = B;
        B = tmp; */

    }

    // calculate sum of C for each process
	for(i=0; i<TILESIZE; i++) {
		for(j=0; j<TILESIZE; j++) {
			send_data += C[i * TILESIZE + j];
		}
	}

    MPI_Reduce(&send_data, &recv_data, 1, MPI_DOUBLE, MPI_SUM, 0, cannon_comm);

    MPI_Barrier(cannon_comm);
    end = MPI_Wtime();

	// Result validation in rank 0
	if(rank == 0) {
		double expected_sum = (pow(N, 3)*pow(N-1, 2))/2;
        printf("tilesize = %d\n", TILESIZE);
		fprintf(stderr, "Expected sum of C: \t%lf\n", expected_sum);
		fprintf(stderr, "Calculate sum of C: \t%lf\n", recv_data);
		if(abs(expected_sum-recv_data) < 0.000000001) fprintf(stderr,"Multiplication Correct\n");
		else fprintf(stderr,"Multiplication Incorrect\n");
	}

    free(A);
    free(B);
    free(buf);
    free(C);

    MPI_Finalize();
    return 0;
}