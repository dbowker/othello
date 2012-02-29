/*
 * Author: Dan Bowker
 * Name: worker.cpp
 * Date: 2/27/12
 * purpose: This is the worker routine to process computer moves 
 */

#include "othello.h"

void worker(Communicator comm) {
	int i;
	int validCMoves[BS];
	int scoreCMoves[BS];

	bool done = false;
	int placeInHistory;
	WorkRequest* wReq = new WorkRequest();
	WorkResult*  wRes = new WorkResult();
	WorkAddition* wAdd = new WorkAddition();
	do {
		// wait for some work
		comm.recv(0, (char*) wReq, sizeof(WorkRequest), TAG_DO_THIS_WORK);
		// if the first character of the board is a "T" then this is not really a
		// work request but rather a sentinel marking "End of Game"
		if (wReq->b[0] == 'T') {
			cout << comm.rank << "\tReceived the terminate sentinel. \n";
			break;
		}
		char otherColor = wReq->color==C?H:C;

		// find our current move.  The current move is the last valid item in
		// history.  History ends at the "0" marker.
		for (placeInHistory=0; wReq->history[placeInHistory]; placeInHistory++);
		placeInHistory--;

		// make the move and update the board
		squareScore(wReq->b, wReq->history[placeInHistory], wReq->color, true);

		// record the score for this board after making our move
		wReq->scores[placeInHistory] = getBoardValue(wReq->b,wReq->color);

//		cout << "for the move of " <<(short) wReq->history[placeInHistory] << " the new board value is " << (short)wReq->scores[placeInHistory] << endl;

		//more depth to compute?
		if (--wReq->depth >= 0) {
			do {
				placeInHistory++;
				// find the one level best move for the human and assume
				// that's the move they will do.
				wReq->history[placeInHistory] = findOneLevelDeepBestMove(wReq->b,otherColor);
				wReq->history[placeInHistory+1] = 0;
				// if human has a valid move - make that move and record the score for the modified board
				if (wReq->history[placeInHistory] != 0) {
					squareScore(wReq->b,wReq->history[placeInHistory],otherColor,true);
					wReq->scores[placeInHistory] = getBoardValue(wReq->b,wReq->color);
//					cout << "human will move to " << (short)wReq->history[placeInHistory] << " the new board value is " <<(short) wReq->scores[placeInHistory] << endl;
				} 
				// get my valid next moves
				validMoves(wReq->b,wReq->color, validCMoves, scoreCMoves);
				// loop back if the computer doesn't have a valid move
			} while (validCMoves[0] == 0 && wReq->history[placeInHistory] != 0);  // C can't move and H did

			// this level has been completed - prepare for next
			placeInHistory++;
			i = 0;
			// send next level back to queue master
			if (validCMoves[i] != 0) {
				makeWorkAddition(wAdd, wReq, validCMoves);
				comm.send(0,(char*) wAdd, sizeof(WorkAddition), TAG_TO_QUEUE);
			} else {
				// no one has any valid moves on this path - return result
				wRes = makeResult(wRes,wReq);
				comm.send(0, (char*) wRes, sizeof(WorkResult), TAG_RESULT);  // no valid moves
			}
		} else {
			// no more levels to process.
			makeResult(wRes, wReq);
			comm.send(0, (char*) wRes, sizeof(WorkResult), TAG_RESULT);  // no valid moves
		}
	} while (!done);
}
