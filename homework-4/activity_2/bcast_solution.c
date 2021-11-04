	if(strcmp(bcast_implementation_name, "default_bcast") == 0) {
		MPI_Bcast(&buffer[0], NUM_BYTES, MPI_BYTE, 0, MPI_COMM_WORLD);
	}
	if(strcmp(bcast_implementation_name, "naive_bcast") == 0) {
		if(rank == 0) {
			for(int i = 1; i< num_procs; i++) {
				MPI_Send(&buffer[0], NUM_BYTES, MPI_BYTE, i, 1, MPI_COMM_WORLD);
			}
		}else {
			MPI_Recv(&buffer[0], NUM_BYTES, MPI_BYTE, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
	}
	if(strcmp(bcast_implementation_name, "ring_bcast") == 0) {
		if(rank == 0) {
			MPI_Send(&buffer[0], NUM_BYTES, MPI_BYTE, rank+1, 1, MPI_COMM_WORLD);
		}
		else {
			MPI_Recv(&buffer[0], NUM_BYTES, MPI_BYTE, rank-1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			if(rank != num_procs-1)  MPI_Send(&buffer[0], NUM_BYTES, MPI_BYTE, rank+1, 1, MPI_COMM_WORLD);
		}
	}
	if(strcmp(bcast_implementation_name, "pipelined_ring_bcast") == 0) {
		int remain = NUM_BYTES;
	    int actual_length;
		chunk_size = strtol(argv[2], NULL, 10);
		
		if(rank == 0) {
			for(int i=0; i<NUM_BYTES; i=i+chunk_size) {
				if((i+chunk_size) > NUM_BYTES) {
					actual_length = (NUM_BYTES % chunk_size);
				} else {
					actual_length = chunk_size;
				}
				MPI_Send(&buffer[i], actual_length, MPI_BYTE, rank+1, 3, MPI_COMM_WORLD);
			}		
		}else {
			for(int j=0; j<NUM_BYTES; j=j+chunk_size) {
				if((j+ chunk_size) > NUM_BYTES) {
					actual_length = (NUM_BYTES % chunk_size);
				} else {
					actual_length = chunk_size;
				}
				MPI_Recv(&buffer[j], actual_length, MPI_BYTE, rank-1, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				if(rank != (num_procs-1)) MPI_Send(&buffer[j], actual_length, MPI_BYTE, rank+1, 3, MPI_COMM_WORLD);
			}
		}
	}