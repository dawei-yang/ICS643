default: bcast_skeleton


bcast_skeleton: bcast_skeleton.c
					docker run --rm -it -v $(dir $(abspath $(lastword $(MAKEFILE_LIST)))):/home/smpi --user $(id -u):$(id -g) henricasanova/ics632_smpi smpicc bcast_skeleton.c -o bcast_skeleton 

run:
	 docker run --rm -it -v $(dir $(abspath $(lastword $(MAKEFILE_LIST)))):/home/smpi --user $(id -u):$(id -g) henricasanova/ics632_smpi smpirun --cfg=smpi/bcast:mpich -np 50 -platform ring_50.xml -hostfile hostfile_50.txt ./bcast_skeleton asynchronous_pipelined_ring_bcast $(ARGS)

