#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/time.h>

struct timeval start, end;

int main(int argc, char *argv[]) {
    if(argc != 2) {
        printf("Usage: ./nontiled <N>\n");
        return -1;
    }


    int p = 0;
    if (argv[1][0] == '-') {
 	    printf("Invalid N argument \"%s\"\n", argv[1]);
   	    return -1;
    }
    for(; argv[1][p] != 0; p++) {
        if (!isdigit(argv[1][p])) {
	        printf("Invalid N argument \"%s\"\n", argv[1]);
            return -1;
	    }
    }
    
    int N = atoi(argv[1]);
    double *arrayA = (double *)malloc(N * N * sizeof(double));
    double *arrayB = (double *)malloc(N * N * sizeof(double));
    int i, j;

    for(i = 0; i < N; i++) {
        for(j = 0; j < N; j++) {
            arrayA[N * i + j] = (double)(i + j);
            arrayB[N * i + j] = (double)(rand());
        }
    }
    printf("test mod: %d ... %d\n", 45/7, 45%7);
    return 0;
}