/* 
 * File:   Communicator.h
 * Author: dan
 *
 * Created on February 9, 2012, 4:41 PM
 */

#ifndef _COMMUNICATOR_H
#define	_COMMUNICATOR_H

#include "mpi.h"

class Communicator {
	public:
		int rank;
		int nprocs;
		Communicator(int &, char** &);
		
		void send(int, char*, int, int);

		void recv(int, char*, int, int);

		float add(int, float);
		
		void probe(int fromWhere, int whichTag, int &actualFrom, int &actualTag);
		
		void finalize();
};

using namespace std;

#endif	/* _COMMUNICATOR_H */

