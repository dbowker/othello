/* 
 * File:   Communicator.cpp
 * Author: dan
 * 
 * Created on February 9, 2012, 4:41 PM
 */

#include "Communicator.h"

class Communicator {
public:
	int rank;
	int nprocs;
	Communicator(int &argc, char** &argv) {
		MPI_Init(&argc, &argv);
		MPI_Comm_rank(MPI_COMM_WORLD, &rank);
		MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
	}
	template<class T>
	void send(int dest, T* data, int size=1, int tag=1) {
		// printf("%d\tsending %d bytes to %d\n",rank, size*sizeof(T), dest);
		MPI_Send((void*)data,size*sizeof(T),MPI_CHAR, dest, tag, MPI_COMM_WORLD);
	}
	template<class T>
	void recv(int source, T* data, int size=1, int tag=1) {
		MPI_Status status;
		// printf("%d\twaiting to receive up to %d bytes from %d\n",rank, size*sizeof(T), source);
		MPI_Recv((void*)data,size*sizeof(T),MPI_CHAR, source, tag, MPI_COMM_WORLD, &status);
		// printf("%d\treceived %d bytes from %d\n",rank, size*sizeof(T), source);
	}
	template<class T>
	void bcast(int root, T* data, int size=1) {
		MPI_Bcast((void*)data,size*sizeof(T), MPI_CHAR, root, MPI_COMM_WORLD);
	}
	float add(int root, float x) {
		float y;
		MPI_Reduce(&x, &y, 1, MPI_FLOAT, MPI_SUM, root, MPI_COMM_WORLD);
		return y;
	}

}


