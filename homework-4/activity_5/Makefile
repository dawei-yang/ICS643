default: bcast_skeleton


bcast_skeleton: bcast_skeleton.c
	docker run --rm -it -v $(dir $(abspath $(lastword $(MAKEFILE_LIST)))):/home/smpi --user $(id -u):$(id -g) henricasanova/ics632_smpi smpicc bcast_skeleton.c -o bcast_skeleton 

run_cross:
	docker run --rm -it -v $(dir $(abspath $(lastword $(MAKEFILE_LIST)))):/home/smpi --user $(id -u):$(id -g) henricasanova/ics632_smpi smpirun --cfg=smpi/bcast:mpich -np 64 -platform cluster_crossbar_64.xml -hostfile hostfile_64.txt ./bcast_skeleton $(ARGS)

run_backbone:
	docker run --rm -it -v $(dir $(abspath $(lastword $(MAKEFILE_LIST)))):/home/smpi --user $(id -u):$(id -g) henricasanova/ics632_smpi smpirun --cfg=smpi/bcast:mpich -np 64 -platform cluster_backbone_64.xml -hostfile hostfile_64.txt ./bcast_skeleton $(ARGS)