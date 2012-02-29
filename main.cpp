/* 
 * File:   main.cpp
 * Author: dan bowker
 * Project: othello
 *
 * Created on February 7, 2012, 7:12 PM
 */

#include "othello.h"

int main(int argc, char** argv) {
	char b[BS];
	int r, c;
	
	Communicator comm(argc, argv);
	int validHMoves[BS];
	int validCMoves[BS];
	int scoreHMoves[BS];
	validCMoves[0] = 0;
	
	if (comm.rank == 0) {
		int hMove = 0;
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
		worker(comm);
	}
	
	comm.finalize();
	
	return (EXIT_SUCCESS);
}

