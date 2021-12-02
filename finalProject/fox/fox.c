#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <string.h>
#include <math.h>

static void program_abort(char *message) {
  int my_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
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
    // MPI_Comm fox_comm;
    MPI_Status status_B;
    MPI_Request request_B;
    int rank, size, step, i, j, k;

    int left, right, up, down;
    double *A, *B, *C, *TmpA, *Buffer;
    double start, end;
    double send_data = 0.0;
	double recv_data = 0.0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(isPerfectSquare(size) == 0) program_abort("Number of processes is not a perfect number\n");
    int p = (int)sqrt(size);
    int TILESIZE = N/p;
	int row_num = rank/p;
	int col_num = rank%p;

    A = (double *)malloc(TILESIZE * TILESIZE * sizeof(double));
    B = (double *)malloc(TILESIZE * TILESIZE * sizeof(double));
    C = (double *)malloc(TILESIZE * TILESIZE * sizeof(double));
    Buffer = (double *)malloc(TILESIZE * TILESIZE * sizeof(double));

    

    for (i = 0; i < TILESIZE; i++) {
        for (j = 0; j < TILESIZE; j++) {
            // use for test: for a 4x4: result should be 231,161
            // A[i * TILESIZE + j] = rand()%100;
            // B[i * TILESIZE + j] = A[i * TILESIZE + j]; 
            A[i * TILESIZE + j] = i + row_num * TILESIZE;
            B[i * TILESIZE + j] = i + row_num * TILESIZE + j + col_num * TILESIZE;
            C[i * TILESIZE + j] = 0.0;
        }
    }


    // Establish row and col sub communicators
	MPI_Comm new_comm_row;
	MPI_Comm new_comm_col;
	MPI_Comm_split(MPI_COMM_WORLD, row_num, col_num, &new_comm_row);
	MPI_Comm_split(MPI_COMM_WORLD, col_num, row_num, &new_comm_col);

    start = MPI_Wtime();
    for (step = 0; step < p; step++) {

        // Broadcase Diagonal el -> row
        TmpA = A;
        int root = (step + row_num)%p;
        MPI_Bcast(&TmpA, 2, MPI_INT, root, new_comm_row);

        // Matrix multiplication
        for (i = 0; i < TILESIZE; i++) {
            for (k = 0; k < TILESIZE; k++) {
                for (j = 0; j < TILESIZE; j++) {
                    C[i * TILESIZE + j] += TmpA[i * TILESIZE + k] * B[k * TILESIZE + j];
                }
            }
        }

        // up shift B
        MPI_Send(B, TILESIZE*TILESIZE, MPI_DOUBLE, (row_num-1 + p)%p, 3, new_comm_col);
        MPI_Recv(Buffer, TILESIZE*TILESIZE, MPI_DOUBLE, (row_num+1)%p, 3, new_comm_col, &status_B);

        // Save B
        for(i=0; i<TILESIZE; i++) {
            for(j=0; j<TILESIZE; j++) {
                B[i * TILESIZE + j] = Buffer[i * TILESIZE + j];
            }
        }
    }

    // calculate sum of C for each process
	for(i=0; i<TILESIZE; i++) {
		for(j=0; j<TILESIZE; j++) {
			send_data += C[i * TILESIZE + j];
		}
	}

    MPI_Reduce(&send_data, &recv_data, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

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
    free(C);
    MPI_Finalize();
    return 0;
}