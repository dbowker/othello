/*
 * File:   findBestMove.cpp
 * Author: dan bowker
 * Project: othello
 *
 * Returns an array of possible moves for the color specified,  The array is
 * passed in as pVM  (pointer_ValidMoves)
 *
 * Created on February 8, 2012, 5:10 PM
 */

#include "othello.h"
#include <list>
#include <queue>


int findBestMove(Communicator comm, char origB[BS], char color, int depth=1) {
	queue<WorkRequest*> workQueue;

	stack<int> availableProcesses;
	int possibleMoves[BS];
	int scores[BS];
	WorkRequest* currentWReqs[comm.nprocs];
	int totalWorkRequests = 0;
	char dummy;
	int i;
	int resultScores[BS];
	int resultCount[BS];

	WorkRequest* wReq;// = new WorkRequest();
	WorkResult*  wRes = new WorkResult();
	
	for(i=1; i < comm.nprocs; i++) {
		availableProcesses.push(i);
                currentWReqs[i] = NULL;
	}
	validMoves(origB, color, possibleMoves, scores);

	// seed the system - push all current valid moves onto the queue
	cout << "Ready to queue possible moves.\n";
	for (i=0; possibleMoves[i]; i++) {
		wReq = new WorkRequest();
		strncpy(wReq->b,origB,BS);
		wReq->color = color;
		wReq->depth = depth;
		wReq->history[1] = 0;

		resultScores[possibleMoves[i]] = 0;
		resultCount[possibleMoves[i]] = 0;

		wReq->history[0] = possibleMoves[i];
		cout << "possibleMoves[i]: " << possibleMoves[i] << endl;
		workQueue.push(wReq);
	}
	
// we've made the initial request - now wait until we get all our answers back
	while (availableProcesses.size() < comm.nprocs-1 || !workQueue.empty()) {
//		workQueue.printQueue();
//		cout << "0\twQ.size: " << workQueue.size() << " avPr.size: " << availableProcesses.size() << endl;
		while (!workQueue.empty() && availableProcesses.size()) {
			wReq = workQueue.front();
			workQueue.pop();
			comm.send(availableProcesses.top(),(char*) wReq, sizeof(WorkRequest), TAG_DO_THIS_WORK);
//			cout << "0\tSent " << wReq->history[0] << " to " << availableProcesses.top() << endl;
			currentWReqs[availableProcesses.top()] = wReq;
			totalWorkRequests++;
			availableProcesses.pop();
		}
		int from;
		int tag;
//		cout << "0\tis about wait anything\n";
		comm.probe(MPI_ANY_SOURCE, MPI_ANY_TAG,from,tag);	// wait for any communication from anywhere
		if (currentWReqs[from] != NULL) {
			delete currentWReqs[from];
			currentWReqs[from] = NULL;
		}
//		cout << "0\tTag: " << tag << " from " << from << endl;
		if (tag == TAG_PROC_AVAILABLE) {
			comm.recv(from, &dummy, 1, tag);
//			cout << "0\tReceived AVAILABLE from " << from << endl;
			availableProcesses.push(from);
//			continue;						// loop back up to allow more work to be done
		}
		if (tag == TAG_TO_QUEUE) {
			wReq = new WorkRequest();
			comm.recv(from,(char*) wReq, sizeof(WorkRequest), tag);
//			cout << "0\tReceived a TO_QUEUE request\n";
			workQueue.push(wReq);  // not actually a result, just more work.
//			cout << comm.rank << "\tqueued this request  color:"<< wReq->color << "  depth:" << wReq->depth << "  history:" << wReq->history[0] << " " << wReq->history[1] << " " << wReq->history[2] << " " << wReq->history[3] << " " << wReq->history[4] << " " << wReq->history[5] << " " << wReq->history[6] << " " << wReq->history[7] << " " << wReq->history[8] << " " << wReq->history[9] << " " << wReq->history[10] << " " << wReq->history[11] << " " << wReq->history[12] << " " << wReq->history[13] << " " << endl;
//			continue;
		}
		if (tag == TAG_RESULT) {
			comm.recv(from,(char*) wRes, sizeof(WorkResult), TAG_RESULT);
//			cout << "0\tReceived a RESULT request\n";
//			cout << "0\tresult's history is: " << " " << wRes->history[0] << " " << wRes->history[1] << " " << wRes->history[2] << " " << wRes->history[3] << endl;
//			results[wRes->history].push_back(wRes);
			resultScores[wRes->history] += wRes->boardValue;
			resultCount[wRes->history]++;
//			cout << "0\tResult: " << wRes->history << " " << wRes->boardValue <<  " from " << from << endl;
		}
//		cout << "0\tap.size():" << availableProcesses.size() << " comm.nprocs-1: " << (comm.nprocs-1) << " wq.size:" << workQueue.size() << endl;
 	}
//	sleep(3);
	cout << "*********************************************" << endl;
	cout << "***    all done processing possible moves  **" << endl;
	cout << "*********************************************" << endl;
	cout << "requests sent: " << totalWorkRequests << endl;
/*
	int bestValue = -99999;
	int bestPosition = 0;
	int j;
	for (i=0; i < BS; i++) {
		if (!results[i].empty()) {
			cout << " results[" << i << "].size: " << results[i].size() << endl;
			wRes = results[i].front();
			results[i].pop_front();
			cout << "Result: boardValue: " << wRes->boardValue  << " history";
			for (j = 0; wRes->history[j];j++) {
				cout << "\t\t" << setw(3) << wRes->history[j] << "\t" << setw(5) << wRes->scores[j] << endl;
			}
			if (bestValue < wRes->history[j-1]) {
				bestValue = wRes->history[j-1];
				bestPosition = wRes->history[0];
			}
		}
	}
*/
	double bestValue = -99999;
	int bestPosition = 0;
	cout << "All votes received:" << endl;
	for (i=0; possibleMoves[i]; i++) {
		int move = possibleMoves[i];
		double finalScore = (double)resultScores[move] / (double)resultCount[move];
		cout << "Position " << move << " final score: " << finalScore << " (" << resultScores[move] << "/" << resultCount[move] << ")" << endl;
		if (bestValue < finalScore) {
			bestValue = finalScore;
			bestPosition = move;
		}
	}
  	cout << "best position appears to be " << bestPosition << endl;

	return bestPosition;
}

