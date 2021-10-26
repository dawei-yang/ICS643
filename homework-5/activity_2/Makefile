default: matmul


matmul: matmul.c
					docker run --rm -it -v $(dir $(abspath $(lastword $(MAKEFILE_LIST)))):/home/smpi --user $(id -u):$(id -g) henricasanova/ics632_smpi smpicc matmul.c -o matmul

run:
	 docker run --rm -it -v $(dir $(abspath $(lastword $(MAKEFILE_LIST)))):/home/smpi --user $(id -u):$(id -g) henricasanova/ics632_smpi smpirun -np 4 -platform cluster_crossbar_64.xml -hostfile hostfile_64.txt ./matmul $(ARGS)

