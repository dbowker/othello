/*
 * Author: Dan Bowker
 * Name processRequest
 * Date:   February 26
 * Purpose: Given a work request from "findBestMove", add it to the queue and 
 * process it.  Originally all moves were passed in, but by processing one
 * request at at time it cuts down the required memory.  Enough so an extra
 * level could potentially be processed.
 */


#include "othello.h"

void processRequest(Communicator comm, WorkRequest* inReq, int &totalWorkRequests, int resultScores[], int resultCount[], int resultMin[], int resultMax[]) {
	int i;
	int from;
	int tag;

	int currentComputationalDepth = 99;

	queue<WorkRequest*> workQueue;
	stack<int> availableProcesses;
	WorkRequest* currentWReqs[comm.nprocs];

	WorkRequest* wReq;// = new WorkRequest();
	WorkResult*  wRes = new WorkResult();
	WorkAddition* wAdd = new WorkAddition();

	for(i=1; i < comm.nprocs; i++) {
		availableProcesses.push(i);
        currentWReqs[i] = NULL;
	}

	workQueue.push(inReq);

// we've made the initial request - now wait until we get all our answers back
	while (availableProcesses.size() < (unsigned int) (comm.nprocs - 1) || !workQueue.empty()) {

		// any work to be done and any processes available?
		while (!workQueue.empty() && availableProcesses.size()) {
			wReq = workQueue.front();
			workQueue.pop();
			// send a piece of work to the first available process
			comm.send(availableProcesses.top(),(char*) wReq, sizeof(WorkRequest), TAG_DO_THIS_WORK);

			// retain request 
			// a - if the process fails we can reissue the request
			// b - so we can free the memory when we get something back
			currentWReqs[availableProcesses.top()] = wReq;
			totalWorkRequests++;
			availableProcesses.pop();
		}
		
		// wait for some response
		comm.probe(MPI_ANY_SOURCE, MPI_ANY_TAG,from,tag);	// wait for any communication from anywhere

		// more work to do?
		if (tag == TAG_TO_QUEUE) {
			comm.recv(from, (char*) wAdd, sizeof(WorkAddition), tag);
			// got more work to do - split request to put on queue for each possible move
			for(i = 0; wAdd->additionalMoves[i]; i++) {
				wReq = new WorkRequest();  // will be deleted when we receive a response
				makeWorkRequest(wReq, wAdd, wAdd->additionalMoves[i]);
				workQueue.push(wReq);  // not actually a result, just more work.
			}
		}
		// final result received  (min and max are not used, just interested in the stats
		if (tag == TAG_RESULT) {
			comm.recv(from,(char*) wRes, sizeof(WorkResult), TAG_RESULT);
			resultScores[(int)wRes->history] += wRes->boardValue;
			resultCount[(int)wRes->history]++;
			if (resultMin[(int)wRes->history] > wRes->min) resultMin[(int)wRes->history] = wRes->min ;
			if (resultMax[(int)wRes->history] < wRes->max) resultMax[(int)wRes->history] = wRes->max ;
		}
		
		// put "from" back into the available processes queue and 
		// free memory associated with it's original request.
		availableProcesses.push(from);
		if (currentWReqs[from] != NULL && currentWReqs[from] != inReq) {
			delete currentWReqs[from];		// free memory for the already processed request.
			currentWReqs[from] = NULL;
		}
 	}

}