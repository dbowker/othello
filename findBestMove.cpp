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

int findBestMove(Communicator comm, char b[BS], char color, int depth=1) {
	static WorkQueue workQueue;

	static stack<int> availableProcesses;

	static char *rb = (char*)malloc(MAX_REQUEST_SIZE);

	static int possibleMoves[BS];
	static int scores[BS];

	int i;
	stack<int> bestPositions;
	int bestScore = 0;
	int bestPosition = 0;
	
	for(i=1; i < comm.nprocs; i++) {
		availableProcesses.push(i);
	}
	
	validMoves(b, color, possibleMoves, scores);

	for (i=0; possibleMoves[i]; i++) {
		scores[possibleMoves[i]] = -1;
		buildWorkRequest(rb,b,color,color,possibleMoves[i],depth,possibleMoves[i]);
		if (workQueue.push(rb))
			cout << "0\tJust queued the possible move of " << possibleMoves[i] << "  Current outstandingwork count: " << workQueue.getOutstandingWork() << endl;
		else
			cout << "0\tWork queue if full\n";
	}
// we've made the initial request - now wait until we get all our answers back
	while (workQueue.getOutstandingWork()) {
		cout << "0\tCurrent outstandingwork count: " << workQueue.getOutstandingWork() << "  size of available processes " << availableProcesses.size() << endl;
		while (!workQueue.isEmpty() && availableProcesses.size()) {
			workQueue.pop(rb);
			comm.send(availableProcesses.top(), rb, strlen(rb)+1, TAG_DO_THIS_WORK);
			availableProcesses.pop();
		}
		int from;
		int tag;
		comm.probe(MPI_ANY_SOURCE, MPI_ANY_TAG,from,tag);
		if (tag == TAG_WORK_TO_DO) {
			comm.recv(from,rb,MAX_REQUEST_SIZE, TAG_WORK_TO_DO);
			
		} else if (tag == TAG_RESULT) {
			int score;
			int move;
			int bufSize = (MOVE_DIGITS+1)*2+1;
			char result[bufSize];  // Return two numbers plus null (leave room for sign)
			char *p = &result[0];
			workQueue.resultReceived();
			comm.recv(from, result, bufSize, TAG_RESULT);
			availableProcesses.push(from);
			score = aToI(p, 2, 10);
			move = aToI(p, 2, 10);
			cout << "0\tResult received from " << from << " origMove:" << move << "  score:" << score << endl;
			scores[move] = max(scores[move],score);
			if (score > bestScore) {
				bestScore = score;
				while (!bestPositions.empty()) bestPositions.pop();
				bestPositions.push(move);
			}
		}
 	}
	if (bestPositions.size()) {
		bestPosition = bestPositions.top();
	} else {
		bestPosition = 0;
	}
	return bestPosition;
}
int findBestMove1(char b[BS], char color, bool flip) {

	int bestPosition = 0;

	int bestMoves[BS];
	int possibleMoves[BS];
	int scores[BS];
	int i;
	int cBM = 0;	// count of best moves

	int best = validMoves(b, color, possibleMoves, scores);

	for (i = 0; possibleMoves[i]; i++) {
		if (scores[i] == best) {
			bestMoves[cBM++] = possibleMoves[i];
		}
	}
	int bestIndex = rand() % cBM;
	bestPosition = bestMoves[bestIndex];
	squareScore(b, bestPosition, color, flip);
	return bestPosition;
}
