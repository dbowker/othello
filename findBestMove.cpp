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
#include <sys/time.h>

int findBestMove(Communicator comm, char origB[BS], char color, int depth) {

	int wallClockTime;
	int processorTime;
	int boardCount;

	int possibleMoves[BS];
	int scores[BS];
	int totalWorkRequests = 0;
	int i;
	int resultScores[BS];
	int resultCount[BS];
	char logBuffer[200];
    struct timeval startTime, endTime;

	gettimeofday(&startTime, NULL);

	
	WorkRequest* wReq;// = new WorkRequest();
	
	validMoves(origB, color, possibleMoves, scores);

	// seed the system - push all current valid moves onto the queue one at a time
	cout << "Ready to process possible moves.\n";
	wReq = new WorkRequest();
	processorTime = 0;
	boardCount = 0;
	wallClockTime = 0;
	for (int whichPossibleMove=0; possibleMoves[whichPossibleMove]; whichPossibleMove++) {
		strncpy(wReq->b,origB,BS);
		wReq->color = color;
		wReq->depth = depth;
		wReq->history[1] = 0;

		resultScores[possibleMoves[whichPossibleMove]] = 0;
		resultCount[possibleMoves[whichPossibleMove]] = 0;
		
		wReq->history[0] = possibleMoves[whichPossibleMove];

		processRequest(comm, wReq, totalWorkRequests, resultScores, resultCount);
		cout << comm.rank << "\tDone processing " << possibleMoves[whichPossibleMove] << endl;
	}
	delete wReq;

	gettimeofday(&endTime, NULL);
	
	int elapsed = elapsedTime(&endTime, &startTime);
	logIt(elapsed);
	
	cout << "*********************************************" << endl;
	cout << "***    all done processing possible moves  **" << endl;
	cout << "*********************************************" << endl;
	cout << "requests sent: " << totalWorkRequests << endl;


	double bestValue = -9.0e100;
	int bestPosition = 0;
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

