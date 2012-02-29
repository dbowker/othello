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
	int i, j;
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
	while (availableProcesses.size() < comm.nprocs-1 || !workQueue.empty()) {
		while (!workQueue.empty() && availableProcesses.size()) {
			wReq = workQueue.front();
			workQueue.pop();
			comm.send(availableProcesses.top(),(char*) wReq, sizeof(WorkRequest), TAG_DO_THIS_WORK);
			currentWReqs[availableProcesses.top()] = wReq;
			totalWorkRequests++;
			availableProcesses.pop();
		}
		
		comm.probe(MPI_ANY_SOURCE, MPI_ANY_TAG,from,tag);	// wait for any communication from anywhere

		if (tag == TAG_TO_QUEUE) {
			comm.recv(from, (char*) wAdd, sizeof(WorkAddition), tag);
			if (currentComputationalDepth > wAdd->depth) {
				currentComputationalDepth = wAdd->depth;
			}
			for(i = 0; wAdd->additionalMoves[i]; i++) {
				wReq = new WorkRequest();  // will be deleted when we receive a response
				makeWorkRequest(wReq, wAdd, wAdd->additionalMoves[i]);
				workQueue.push(wReq);  // not actually a result, just more work.
			}
		}
		if (tag == TAG_RESULT) {
			comm.recv(from,(char*) wRes, sizeof(WorkResult), TAG_RESULT);
			resultScores[wRes->history] += wRes->boardValue;
			resultCount[wRes->history]++;
			if (resultMin[wRes->history] > wRes->min) resultMin[wRes->history] = wRes->min ;
			if (resultMax[wRes->history] < wRes->max) resultMax[wRes->history] = wRes->max ;
		}
		
		availableProcesses.push(from);
		if (currentWReqs[from] != NULL && currentWReqs[from] != inReq) {
			delete currentWReqs[from];		// free memory for the already processed request.
			currentWReqs[from] = NULL;
		}
 	}

}