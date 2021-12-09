#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <string.h>
#include <math.h>
#define FLOP_CALIBRATION_FACTOR 21.2443

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
    int rank, size, shift, i, j, k;
    int dims[2];
    int periods[2];
    int left, right, up, down;
    double *A, *B, *C;
     double start, end, start_1, end_1, start_2, start_3;
    double send_data = 0.0;
	double recv_data = 0.0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(isPerfectSquare(size) == 0) program_abort("Number of processes is not a perfect number\n");
    int p = (int)sqrt(size);
    dims[0] = p;
    dims[1] = p;
    periods[0] = 1;
    periods[1] = 1;
    int tile_size = N/p;
	int row_num = rank/p;
	int col_num = rank%p;

    A = (double *)SMPI_SHARED_MALLOC(tile_size * tile_size * sizeof(double));
    B = (double *)SMPI_SHARED_MALLOC(tile_size * tile_size * sizeof(double));
    C = (double *)SMPI_SHARED_MALLOC(tile_size * tile_size * sizeof(double));

    for (i = 0; i < tile_size; i++) {
        for (j = 0; j < tile_size; j++) {
            // use for test: for a 4x4: result should be 321,161
            // A[i * tile_size + j] = rand()%100;
            // B[i * tile_size + j] = A[i * tile_size + j]; 
            A[i * tile_size + j] = i + row_num * tile_size;
            B[i * tile_size + j] = i + row_num * tile_size + j + col_num * tile_size;
            C[i * tile_size + j] = 0.0;
        }
    }

    if (rank == 0) start = MPI_Wtime();
    // Initial skew
    
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 1, &cannon_comm);
    MPI_Cart_shift(cannon_comm, 1, row_num, &left, &right);
    MPI_Sendrecv_replace(&(A[0]), tile_size * tile_size, MPI_DOUBLE, left, 1, right, 1, cannon_comm, MPI_STATUS_IGNORE);
	MPI_Cart_shift(cannon_comm, 0, col_num, &up, &down);
	MPI_Sendrecv_replace(&(B[0]), tile_size * tile_size, MPI_DOUBLE, up, 1, down, 1, cannon_comm, MPI_STATUS_IGNORE);
    
    if (rank == 0) fprintf(stdout, "skew time %lf\n", MPI_Wtime() - start);

    // MPI Cartesian
    MPI_Cart_shift(cannon_comm, 0, 1, &left, &right);
    MPI_Cart_shift(cannon_comm, 1, 1, &up, &down);

  
    for (shift = 0; shift < p; shift++) {
        if (rank == 1 && shift == 0) {start_3 = MPI_Wtime();}
        double flops = (double)tile_size*(double)tile_size*(double)tile_size * (double)FLOP_CALIBRATION_FACTOR ;
        SMPI_SAMPLE_FLOPS(flops){
        /* for (i = 0; i < tile_size; i++) {
            for (k = 0; k < tile_size; k++) {
                for (j = 0; j < tile_size; j++) {
                    C[i * tile_size + j] += A[i * tile_size + k] * B[k * tile_size + j];
                    // printf("rank [%d], C[%d] = %lf\n", rank, i*tile_size+j, C[i * tile_size + j]);
                }
            } */
        }
        if (rank == 1 && shift == 0) {printf("--> %lf\n", MPI_Wtime() - start_3);}
        // Communication
        if(shift == 1 && rank == 1) start_2 = MPI_Wtime();
        //MPI_Sendrecv_replace(A, tile_size*tile_size, MPI_DOUBLE, left, 1, right, 1, cannon_comm, MPI_STATUS_IGNORE);
		//MPI_Sendrecv_replace(B, tile_size*tile_size, MPI_DOUBLE, up, 1, down, 1, cannon_comm, MPI_STATUS_IGNORE);

        MPI_Isend(A, tile_size * tile_size, MPI_DOUBLE, left, 1, cannon_comm, &request_A1);
        MPI_Sendrecv_replace(B, tile_size*tile_size, MPI_DOUBLE, up, 1, down, 1, cannon_comm, MPI_STATUS_IGNORE);
        MPI_Recv(A, tile_size* tile_size, MPI_DOUBLE, right, 1, cannon_comm, MPI_STATUS_IGNORE);
        if(shift == 1 && rank == 1) fprintf(stdout, "send and recv time: %lf\n", MPI_Wtime() - start_2);
    }

    // calculate sum of C for each process
	for(i=0; i<tile_size; i++) {
		for(j=0; j<tile_size; j++) {
			send_data += C[i * tile_size + j];
		}
	}

    MPI_Reduce(&send_data, &recv_data, 1, MPI_DOUBLE, MPI_SUM, 0, cannon_comm);

    MPI_Barrier(cannon_comm);

	// Result validation in rank 0
	if(rank == 0) {
		double expected_sum = (pow(N, 3)*pow(N-1, 2))/2;
        fprintf(stdout, "Total time: %lf\n", MPI_Wtime() - start);
		fprintf(stderr, "Expected sum of C: \t%lf\n", expected_sum);
		fprintf(stderr, "Calculate sum of C: \t%lf\n", recv_data);
		if(abs(expected_sum-recv_data) < 0.000000001) fprintf(stderr,"Multiplication Correct\n");
		else fprintf(stderr,"Multiplication Incorrect\n");
	}

    SMPI_SHARED_FREE(A);
    SMPI_SHARED_FREE(B);
    SMPI_SHARED_FREE(C);

	MPI_Comm_free(&cannon_comm);
    MPI_Finalize();
    return 0;
}