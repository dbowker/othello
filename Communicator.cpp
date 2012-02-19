/* 
 * File:   Communicator.cpp
 * Author: dan
 * 
 * Created on February 9, 2012, 4:41 PM
 */

#include "othello.h"
 
Communicator::Communicator(int &argc, char** &argv) {
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
}

void Communicator::send(int dest, char* data, int size=1, int tag=1) {
//	cout << rank << "\tsending " << size << " chars to " << dest << endl;
	
	MPI_Send((void*)data,size,MPI_CHAR, dest, tag, MPI_COMM_WORLD);
}

void Communicator::recv(int source, char* data, int size=1, int tag=1) {
	MPI_Status status;
//	cout << rank << "\treceiving up to " << size << "bytes from " << source  << endl;
	MPI_Recv((void*)data,size, MPI_CHAR, source, tag, MPI_COMM_WORLD, &status);
//	cout << rank << "\t received data.\n";
}


float Communicator::add(int root, float x) {
	float y;
	MPI_Reduce(&x, &y, 1, MPI_FLOAT, MPI_SUM, root, MPI_COMM_WORLD);
	return y;
}

void Communicator::finalize() {
	MPI_Finalize();
}

void Communicator::probe(int fromWhere, int whichTag, int &actualFrom, int &actualTag) {
	MPI_Status status;
	MPI_Probe(fromWhere, whichTag, MPI_COMM_WORLD, &status);
	actualFrom = status.MPI_SOURCE;
	actualTag  = status.MPI_TAG;
}

