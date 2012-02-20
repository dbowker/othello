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
		cout << "RUnning depth: " << depth << endl;
		
		srand((int) time(NULL));
		setupBoard(b);
		do {
			validMoves(b,H,validHMoves,scoreHMoves);
			displayBoard(b);
			if (validHMoves[0]) {
				hMove = moveHuman(b,validHMoves);
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
				sleep(2);
				cout << "Computer moves to [" << r << "," << c << "]\n";
			} else {
				sleep(2);
				cout << "Computer does not have a valid move\n";
			}
		} while ((validCMoves[0] || validHMoves[0]) && (hMove != 0));

		char terminate = 'T';
		for(int i = 1; i < comm.nprocs; i++)
			comm.send(i,&terminate,1,TAG_TERMINATE);
		cout << "\n\nNO more valid moves\n\n";
	} else { // not comm.rank 0
		bool done = false;
		WorkRequest* wReq = new WorkRequest();
                WorkResult*  wRes = new WorkResult();
		do {
			int from, tag;
			comm.probe(0,MPI_ANY_TAG,from,tag);
			if (tag == TAG_TERMINATE)
				break;
			
			comm.recv(0, (char*) wReq, sizeof(WorkRequest), TAG_DO_THIS_WORK);
		
			char otherColor = wReq->color==C?H:C;
			int placeInHistory;
			for (placeInHistory=0; wReq->history[placeInHistory]; placeInHistory++);
			placeInHistory--;
//			cout << comm.rank << "\treceived this request  color:"<< wReq->color << "  depth:" << wReq->depth << "  history:" << wReq->history[0] << " " << wReq->history[1] << " " << wReq->history[2] << " " << wReq->history[3] << " " << wReq->history[4] << " " << wReq->history[5] << " " << wReq->history[6] << " " << wReq->history[7] << " " << wReq->history[8] << " " << wReq->history[9] << " " << wReq->history[10] << " " << wReq->history[11] << " " << wReq->history[12] << " " << wReq->history[13] << " " << endl;
			squareScore(wReq->b, wReq->history[placeInHistory], wReq->color, true);
			wReq->scores[placeInHistory] = getBoardValue(wReq->b,wReq->color);
//			sleep(1);
//			cout << "after moving choice - board is:" << endl;
//			displayBoard(wReq->b);
//			cout <<  wReq->color << " just moved to " << wReq->history[placeInHistory] << " and the board value is" << wReq->scores[placeInHistory] << endl;
//			sleep(1);
			if (--wReq->depth >= 0) {
				do {
					placeInHistory++;
					wReq->history[placeInHistory] = findOneLevelDeepBestMove(wReq->b,otherColor);
					wReq->history[placeInHistory+1] = 0;
//					cout << "finding one level deep for " << otherColor << " resulted in " << wReq->history[placeInHistory] << endl;
					if (wReq->history[placeInHistory] != 0) {
						squareScore(wReq->b,wReq->history[placeInHistory],otherColor,true);
//						cout << "after moving to " << wReq->history[placeInHistory] << " the board looks like this:" << endl;
//						displayBoard(wReq->b);
						wReq->scores[placeInHistory] = getBoardValue(wReq->b,wReq->color);
//						cout << "with a board value of " << wReq->scores[placeInHistory] << endl;
//						sleep(1);
					}
					validMoves(wReq->b,wReq->color, validCMoves, scoreCMoves);
//					cout << "After Moving other player our valid move [0] = " << validCMoves[0] << endl;
				} while (validCMoves[0] == 0 && wReq->history[placeInHistory] != 0);
				placeInHistory++;
				i = 0;
				if (validCMoves[i] != 0) {
					while (validCMoves[i] != 0) {
//						cout << comm.rank << "\tsending request for more work on " << wReq->history[0] << endl;
						wReq->history[placeInHistory] = validCMoves[i++];
						wReq->history[placeInHistory+1] = 0;
						comm.send(0,(char*) wReq, sizeof(WorkRequest), TAG_TO_QUEUE);
					}
				} else { // for originalColor stop here
					wRes = makeResult(wRes,wReq);
					comm.send(0, (char*) wRes, sizeof(WorkResult), TAG_RESULT);  // no valid moves
				}
			} else {
				makeResult(wRes, wReq);
//				sleep(1);
//				cout << "this is the last depth returning:\n";
//				cout << "boardValue: " << wRes->boardValue << endl;
//				cout << "history/scores" << endl;
//				cout << wRes->history[0] << "/" << wRes->scores[0] << endl;
//				cout << wRes->history[1] << "/" << wRes->scores[1] << endl;
//				cout << wRes->history[2] << "/" << wRes->scores[2] << endl;
//				cout << wRes->history[3] << "/" << wRes->scores[3] << endl;
//				sleep(1);
				comm.send(0, (char*) wRes, sizeof(WorkResult), TAG_RESULT);  // no valid moves
			}
//			cout << comm.rank << "\tSending Available.\n";
			comm.send(0,&dummy, 1, TAG_PROC_AVAILABLE);
		} while (!done);
	}
	
	comm.finalize();
	
	return (EXIT_SUCCESS);
}

