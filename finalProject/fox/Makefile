default: fox


fox: fox.c
	docker run --rm -it -v $(dir $(abspath $(lastword $(MAKEFILE_LIST)))):/home/smpi --user $(id -u):$(id -g) henricasanova/ics632_smpi smpicc fox.c -o fox

run:
	docker run --rm -it -v $(dir $(abspath $(lastword $(MAKEFILE_LIST)))):/home/smpi --user $(id -u):$(id -g) henricasanova/ics632_smpi smpirun -np 16 -platform cluster_crossbar_64.xml -hostfile hostfile_64.txt ./fox $(ARGS)

