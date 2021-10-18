default: bcast_skeleton


bcast_skeleton: bcast_skeleton.c
					docker run --rm -it -v $(dir $(abspath $(lastword $(MAKEFILE_LIST)))):/home/smpi --user $(id -u):$(id -g) henricasanova/ics632_smpi smpicc bcast_skeleton.c -o bcast_skeleton 

run50:
	 docker run --rm -it -v $(dir $(abspath $(lastword $(MAKEFILE_LIST)))):/home/smpi --user $(id -u):$(id -g) henricasanova/ics632_smpi smpirun --cfg=smpi/bcast:mpich -np 50 -platform ring_50.xml -hostfile hostfile_50.txt ./bcast_skeleton pipelined_ring_bcast $(ARGS)

run35:
	 docker run --rm -it -v $(dir $(abspath $(lastword $(MAKEFILE_LIST)))):/home/smpi --user $(id -u):$(id -g) henricasanova/ics632_smpi smpirun --cfg=smpi/bcast:mpich -np 35 -platform ring_35.xml -hostfile hostfile_35.txt ./bcast_skeleton pipelined_ring_bcast $(ARGS)
run20:
	 docker run --rm -it -v $(dir $(abspath $(lastword $(MAKEFILE_LIST)))):/home/smpi --user $(id -u):$(id -g) henricasanova/ics632_smpi smpirun --cfg=smpi/bcast:mpich -np 20 -platform ring_20.xml -hostfile hostfile_20.txt ./bcast_skeleton pipelined_ring_bcast $(ARGS)
