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
	int bestPosition = 0;

	for(i=1; i < comm.nprocs; i++) {
		availableProcesses.push(i);
	}
	
	validMoves(b, color, possibleMoves, scores);

	for (i=0; possibleMoves[i]; i++) {
		scores[possibleMoves[i]] = 0;
		buildWorkRequest(rb,b,color,color,depth,possibleMoves[i]);
		workQueue.push(rb);
	}
// we've made the initial request - now wait until we get all our answers back
	while (workQueue.getOutstandingWork()) {
		while (!workQueue.isEmpty() && availableProcesses.size()) {
			workQueue.pop(rb);
			comm.send(availableProcesses.top(), rb, strlen(rb)+1, TAG_DO_THIS_WORK);
			availableProcesses.pop();
		}
		cout << "0\tWaiting for any source any tag\n";
		int from;
		int tag;
		comm.probe(MPI_ANY_SOURCE, MPI_ANY_TAG,from,tag);
		if (tag == )
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

	int numWithBest = 0;
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
