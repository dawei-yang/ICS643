default: matmul_init


matmul_init: matmul_init.c
					docker run --rm -it -v $(dir $(abspath $(lastword $(MAKEFILE_LIST)))):/home/smpi --user $(id -u):$(id -g) henricasanova/ics632_smpi smpicc matmul_init.c -o matmul_init 

run:
	 docker run --rm -it -v $(dir $(abspath $(lastword $(MAKEFILE_LIST)))):/home/smpi --user $(id -u):$(id -g) henricasanova/ics632_smpi smpirun -np 4 -platform cluster_crossbar_64.xml -hostfile hostfile_64.txt ./matmul_init $(ARGS)

