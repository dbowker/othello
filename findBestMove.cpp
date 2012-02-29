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


int findBestMove(Communicator comm, char origB[BS], char color, int depth=1) {
	queue<WorkRequest*> workQueue;

	int possibleMoves[BS];
	int scores[BS];
	int totalWorkRequests = 0;
	int i;
	int resultScores[BS];
	int resultMax[BS];
	int resultMin[BS];
	int resultCount[BS];
	
	WorkRequest* wReq;// = new WorkRequest();
	
	validMoves(origB, color, possibleMoves, scores);

	// seed the system - push all current valid moves onto the queue one at a time
	cout << "Ready to process possible moves.\n";
	wReq = new WorkRequest();
	for (int whichPossibleMove=0; possibleMoves[whichPossibleMove]; whichPossibleMove++) {
		strncpy(wReq->b,origB,BS);
		wReq->color = color;
		wReq->depth = depth;
		wReq->history[1] = 0;

		resultScores[possibleMoves[whichPossibleMove]] = 0;
		resultCount[possibleMoves[whichPossibleMove]] = 0;
		resultMin[possibleMoves[whichPossibleMove]] = LONG_MAX;
		resultMax[possibleMoves[whichPossibleMove]] = -LONG_MAX;
		
		wReq->history[0] = possibleMoves[whichPossibleMove];
		//
		// originally I had this out side of this loop.  But by reducing the number of
		// initial states to start off - I can get an extra level of depth before
		// memory is exhausted.
		processRequest(comm, wReq, totalWorkRequests, resultScores, resultCount, resultMin, resultMax);
		cout << comm.rank << "\tDone processing " << possibleMoves[whichPossibleMove] << endl;
	}
	delete wReq;
	
	cout << "*********************************************" << endl;
	cout << "***    all done processing possible moves  **" << endl;
	cout << "*********************************************" << endl;
	cout << "requests sent: " << totalWorkRequests << endl;

	double bestValue = -9.0e100;
	int bestPosition = 0;
	for (i=0; possibleMoves[i]; i++) {
		int move = possibleMoves[i];
		double finalScore = (double)resultScores[move] / (double)resultCount[move];
		cout << "Position " << move << " final score: " << finalScore << " (" << resultScores[move] << "/" << resultCount[move] << ")" << " [" << resultMin[move] << "," << resultMax[move] << "]" << endl;
		if (bestValue < finalScore) {
			bestValue = finalScore;
			bestPosition = move;
		}
	}
  	cout << "best position appears to be " << bestPosition << endl;

	return bestPosition;
}

