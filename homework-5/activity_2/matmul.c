#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>
#include <string.h>
#include <math.h>

// See for the (bad) default random number generator
#define RAND_SEED 842270

// Number of bytes to broadcast

#define NUM_BYTES 100000000

///////////////////////////////////////////////////////
//// program_abort() and print_usage() functions //////
///////////////////////////////////////////////////////

static void program_abort(char *exec_name, char *message);
static void print_usage();

// Abort, printing the usage information only if the
// first argument is non-NULL (and hopefully set to argv[0]), and
// printing the second argument regardless.
static void program_abort(char *exec_name, char *message) {
  int my_rank;
  MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
  if (my_rank == 0) {
    if (message) {
      fprintf(stderr,"%s",message);
    }
    if (exec_name) {
      print_usage(exec_name);
    }
  }
  MPI_Abort(MPI_COMM_WORLD, 1);
  exit(1);
}

// Print the usage information
static void print_usage(char *exec_name) {
  int my_rank;
  MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);

  if (my_rank == 0) {
    fprintf(stderr,"Usage: smpirun --cfg=smpi/bcast:mpich -np <num processes>\n");
    fprintf(stderr,"              -platform <XML platform file> -hostfile <host file>\n");
    fprintf(stderr,"              %s <bcast implementation name> [-c <chunk size>]\n",exec_name);
    fprintf(stderr,"MPIRUN arguments:\n");
    fprintf(stderr,"\t<num processes>: number of MPI processes\n");
    fprintf(stderr,"\t<XML platform file>: a Simgrid platform description file\n");
    fprintf(stderr,"\t<host file>: MPI host file with host names from the platform file\n");
    fprintf(stderr,"PROGRAM arguments:\n");
    fprintf(stderr,"\t<bcast implementation name>: the name of the broadcast implementaion (e.g., naive_bcast)\n");
    fprintf(stderr,"\t[-c <chunk size>]: chunk size in bytes for message splitting (optional)\n");
    fprintf(stderr,"\n");
  }
  return;
}

///////////////////////////
////// Main function //////
///////////////////////////

int main(int argc, char *argv[])
{
	int N;

	// Parse command-line arguments (not using getopt because not thread-safe
	// and annoying anyway). The code below ignores extraneous command-line
	// arguments, which is lame, but we're not in the business of developing
	// a cool thread-safe command-line argument parser.

	MPI_Init(&argc, &argv);

	// Bcast implementation name
	if (argc < 2) {
		program_abort(argv[0],"Missing <Dimension of Matrix N> argument\n");
	} else {
		N = strtol(argv[1], NULL, 10);
	}

	// Determine rank and number of processes
	int num_procs;
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

	int buffer[2] = {0, 0};
	int tile_size =  N/(sqrt(num_procs));

	if (0 == rank) { 
		fprintf(stderr,"N = [%d]\n",N);	
		int dist = 1;
		for (int i = 0; i < N; i+= tile_size) {
			for (int j = 0; j < N; j+= tile_size) {
				buffer[0] = i;
				buffer[1] = j;
				if( !(i == 0 && j == 0)) MPI_Send(&buffer, 2, MPI_INT, dist++, 0, MPI_COMM_WORLD);
			}
		}
	}else {
		MPI_Recv(&buffer, 2, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		double** A = (double**)malloc(tile_size*sizeof(double*));
		double** B = (double**)malloc(tile_size*sizeof(double*));

		for(int i=0; i< tile_size; i++) {
			A[i] = (double*)malloc(tile_size*sizeof(double));
			B[i] = (double*)malloc(tile_size*sizeof(double));
		}
		for(int i=0; i<tile_size; i++) {
			for(int j=0; j<tile_size; j++) {
				A[i][j] = i + buffer[0];
			}
		}
		int global_i = buffer[0]/tile_size;
		int global_j = buffer[1]/tile_size;

/* 		for(int i=0; i<tile_size; i++) {
			for(int j=0; j<tile_size; j++) {
				printf("%.2lf\t", A[i][j]);
			}
			printf("\n");
		} */
		for(int i=0; i<tile_size; i++) {
			for(int j=0; j<tile_size; j++) {
				B[i][j] = i + buffer[0] + j + buffer[1];
			}
		}

/* 		for(int i=0; i<tile_size; i++) {
			for(int j=0; j<tile_size; j++) {
				printf("%.2lf\t", B[i][j]);
			}
			printf("\n");
		} */

	}


	if(rank == 0) {
		MPI_Comm new_comm;
		MPI_Comm_split(MPI_COMM_WORLD, 0, 0, &new_comm);
		int new_rank;
		MPI_Comm_rank(new_comm, &new_rank);
	}
	else {
		int global_i = buffer[0]/tile_size;
		int global_j = buffer[1]/tile_size;
		MPI_Comm new_comm;
		MPI_Comm_split(MPI_COMM_WORLD, global_j, global_j, &new_comm);
		int new_rank;
		MPI_Comm_rank(new_comm, &new_rank);
		printf("rank %d has new rank with color [%d] and key [%d]\n", rank, global_i, global_j);
		printf("comm: %u\n", new_comm);
	}

	MPI_Finalize();
	return 0;
}