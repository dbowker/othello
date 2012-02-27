/* 
 * File:   main.cpp
 * Author: dan bowker
 * Project: othello
 *
 * Created on February 7, 2012, 7:12 PM
 */

#include "othello.h"

int main(int argc, char** argv) {
	char dummy = ' ';
	char b[BS];
	int r, c, i;
	
	Communicator comm(argc, argv);
	int validHMoves[BS];
	int validCMoves[BS];
	int scoreHMoves[BS];
	int scoreCMoves[BS];

	if (comm.rank == 0) {
		int *pVM;
		int hMove;
		bool validMove;

		int depth = atoi(argv[1]);
		
		srand((int) time(NULL));
		setupBoard(b);
		do {
			validMoves(b,H,validHMoves,scoreHMoves);
			displayBoard(b);
			if (validHMoves[0]) {
				hMove = moveHuman(b,validHMoves);
				if (hMove == 0) {
					break;
				}
				cout << "After your move the board is:\n";
				displayBoard(b,C);
			} else {
				cout << "You Don't have any valid moves.\n";
			}
			cout << "Computer is plotting your demise.\n";

			int best = findBestMove(comm,b,C,depth);

			if (best != 0) {
				squareScore(b,best,C,true);
				aiToBs(best, r, c);
				cout << "Computer moves to [" << r << "," << c << "]\n";
			} else {
				cout << "Computer does not have a valid move\n";
			}
		} while ((validCMoves[0] || validHMoves[0]) && (hMove != 0));

		WorkRequest* wReq = new WorkRequest();
		wReq->b[0] = 'T';
		for(int i = 1; i < comm.nprocs; i++)
			comm.send(i, (char *) wReq, 1, TAG_DO_THIS_WORK);
		cout << "\n\nGame over\n\n";
		
	} else { // not comm.rank 0
		bool done = false;
		int placeInHistory;
		WorkRequest* wReq = new WorkRequest();
        WorkResult*  wRes = new WorkResult();
		WorkAddition* wAdd = new WorkAddition();
		int from, tag;
		do {
//			comm.probe(0,MPI_ANY_TAG,from,tag);
//			cout << comm.rank << "\tWaiting for work\n";
			comm.recv(0, (char*) wReq, sizeof(WorkRequest), TAG_DO_THIS_WORK);
			if (wReq->b[0] == 'T') {
				cout << comm.rank << "\tReceived the terminate sentinel. \n";
				break;
			}
			char otherColor = wReq->color==C?H:C;

			for (placeInHistory=0; wReq->history[placeInHistory]; placeInHistory++);
			placeInHistory--;
			
			squareScore(wReq->b, wReq->history[placeInHistory], wReq->color, true);
			wReq->scores[placeInHistory] = getBoardValue(wReq->b,wReq->color);

			//more depth to compute?
			if (--wReq->depth >= 0) {
				do {
					placeInHistory++;
					// find the one level best move for the human and assume
					// that's the move they will do.
					wReq->history[placeInHistory] = findOneLevelDeepBestMove(wReq->b,otherColor);
					wReq->history[placeInHistory+1] = 0;
					// if human has a valid move - make that move
					if (wReq->history[placeInHistory] != 0) {
						squareScore(wReq->b,wReq->history[placeInHistory],otherColor,true);
						wReq->scores[placeInHistory] = getBoardValue(wReq->b,wReq->color);
					}
					// get my valid next moves
					validMoves(wReq->b,wReq->color, validCMoves, scoreCMoves);

				} while (validCMoves[0] == 0 && wReq->history[placeInHistory] != 0);  // as long as H or C had valid move

				placeInHistory++;
				i = 0;
				// send next level back to queue master
				if (validCMoves[i] != 0) {
/* removed to send in one shot
					while (validCMoves[i] != 0) {
						wReq->history[placeInHistory] = validCMoves[i++];
						wReq->history[placeInHistory+1] = 0;
						comm.send(0,(char*) wReq, sizeof(WorkRequest), TAG_TO_QUEUE);
 					}
 */
					makeWorkAddition(wAdd, wReq, validCMoves);
					comm.send(0,(char*) wAdd, sizeof(WorkAddition), TAG_TO_QUEUE);
				} else {
					// we don't have any valid moves on this path - return result
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
	
	comm.finalize();
	
	return (EXIT_SUCCESS);
}

