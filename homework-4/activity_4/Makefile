default: asynchronous_pipelined_bintree_bcast


asynchronous_pipelined_bintree_bcast: asynchronous_pipelined_bintree_bcast.c
					docker run --rm -it -v $(dir $(abspath $(lastword $(MAKEFILE_LIST)))):/home/smpi --user $(id -u):$(id -g) henricasanova/ics632_smpi smpicc asynchronous_pipelined_bintree_bcast.c -o asynchronous_pipelined_bintree_bcast 

run50:
	 docker run --rm -it -v $(dir $(abspath $(lastword $(MAKEFILE_LIST)))):/home/smpi --user $(id -u):$(id -g) henricasanova/ics632_smpi smpirun --cfg=smpi/bcast:mpich -np 50 -platform bintree_50.xml -hostfile hostfile_50.txt ./asynchronous_pipelined_bintree_bcast asynchronous_pipelined_bintree_bcast $(ARGS)

run20:
	 docker run --rm -it -v $(dir $(abspath $(lastword $(MAKEFILE_LIST)))):/home/smpi --user $(id -u):$(id -g) henricasanova/ics632_smpi smpirun --cfg=smpi/bcast:mpich -np 20 -platform bintree_20.xml -hostfile hostfile_20.txt ./asynchronous_pipelined_bintree_bcast asynchronous_pipelined_bintree_bcast $(ARGS)

run7:
	 docker run --rm -it -v $(dir $(abspath $(lastword $(MAKEFILE_LIST)))):/home/smpi --user $(id -u):$(id -g) henricasanova/ics632_smpi smpirun --cfg=smpi/bcast:mpich -np 7 -platform bintree_7.xml -hostfile hostfile_7.txt ./asynchronous_pipelined_bintree_bcast asynchronous_pipelined_bintree_bcast $(ARGS)