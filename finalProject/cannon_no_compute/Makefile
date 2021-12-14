default: cannon


cannon: cannon.c
	docker run --rm -it -v $(dir $(abspath $(lastword $(MAKEFILE_LIST)))):/home/smpi --user $(id -u):$(id -g) henricasanova/ics632_smpi smpicc cannon.c -o cannon

run:
	docker run --rm -it -v $(dir $(abspath $(lastword $(MAKEFILE_LIST)))):/home/smpi --user $(id -u):$(id -g) henricasanova/ics632_smpi smpirun --cfg=smpi/host-speed:5371093750.00 -np 1 -platform cluster_1600.xml -hostfile hostfile_1600.txt ./cannon $(ARGS)

