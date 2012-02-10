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
	if (comm.rank == 0) {
		int validHMoves[BS];
		int validCMoves[BS];
		int scoreHMoves[BS];
		int scoreCMoves[BS];
		int *pVM;
		int hMove;
		bool validMove;

		srand((int) time(NULL));
		setupBoard(b);

		do {
			displayBoard(b);
			validMoves(b,H,validHMoves,scoreHMoves);
			validMoves(b,C,validCMoves,scoreCMoves);
			if (validHMoves[0]) {
				do {
					cout << "Your move [r,c]";
					cin >> r >> c;
					if (r == 0 || c == 0)
						break;
					hMove = bsToAi(r,c);
					validMove = false;
					pVM = validHMoves;
					while (*pVM && !validMove) {
						validMove = (*pVM++ == hMove);
					}
					if (!validMove)
						cout << "\nNot a valid move.\nThe * on the board marks your possible moves\n\n";
					else
						squareScore(b,hMove,H,true);
					int best = findBestMove(comm,b,C,1);
					squareScore(b,best,C,true);
					aiToBs(best, r, c);
					cout << "Computer moves to [" << r << "," << c << "]\n";
				} while (!validMove);
			}
		} while ((validCMoves[0] || validHMoves[0]) && (r != 0 && c != 0));

		cout << "\n\nNO more valid moves\n\n";
	} else { // not comm.rank 0
		bool done = false;
		char* rb = (char*)malloc(MAX_REQUEST_SIZE);
		do {
			comm.recv(0, &rb[0], MAX_REQUEST_SIZE, TAG_DO_THIS_WORK);
			cout << comm.rank << "\treceived this request for work " << rb << endl;
		} while (!done);
	}
	
	comm.finalize();
	
	return (EXIT_SUCCESS);
}

