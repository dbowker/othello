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

		int depth = atoi(argv[1]);
		cout << "RUnning depth: " << depth << endl;
		
		srand((int) time(NULL));
		setupBoard(b);
		do {
			validMoves(b,H,validHMoves,scoreHMoves);
			validMoves(b,C,validCMoves,scoreCMoves);
			displayBoard(b);
			if (validHMoves[0]) {
				do {
					cout << "Your move [r,c]";
					cin >> r >> c;
					if (r == 0 || c == 0) break;

					hMove = bsToAi(r,c);
					cout << "You have chosen to move to " << hMove << endl;
					validMove = false;
					pVM = validHMoves;
					while (*pVM && !validMove) {
						validMove = (*pVM++ == hMove);
					}
					if (!validMove) {
						cout << "\nNot a valid move.\nThe * on the board marks your possible moves\n\n";
						displayBoard(b);
					}
				} while (!validMove);
				if (r == 0 || c == 0) break;

				squareScore(b,hMove,H,true);
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
		} while ((validCMoves[0] || validHMoves[0]) && (r != 0 && c != 0));
		char terminate = 'T';
		for(int i = 1; i < comm.nprocs; i++)
			comm.send(i,&terminate,1,TAG_TERMINATE);
		cout << "\n\nNO more valid moves\n\n";
	} else { // not comm.rank 0
		bool done = false;
		char* rb = (char*)malloc(MAX_REQUEST_SIZE);
		do {
			int tempPossibleMoves[BS];
			int possibleMoves[BS];
			int scores[BS];
			int from, tag;
			comm.probe(0,MPI_ANY_TAG,from,tag);
			if (tag == TAG_TERMINATE)
				break;
			
			comm.recv(0, &rb[0], MAX_REQUEST_SIZE, TAG_DO_THIS_WORK);
			char color;
			char origColor;
			int  origMove;
			int  depth;
			int  move;
			parseRequest(rb, b, color, origColor, origMove, depth, move);
//			cout << comm.rank << "\treceived this request  color:"<< color << " origColor:"<< origColor << "  origMove:" << origMove << "  depth:" << depth << "  move:" << move << endl;;
			squareScore(b,move,color,true);
			cout << "checking to see if good choice for " << color << " board after move: (origColor: " << origColor << ")\n";
			displayBoard(b);
			if (--depth) {
				char otherColor = (color==C)?H:C;
				int highestScore = validMoves(b, otherColor, tempPossibleMoves, scores);
				cout << "valid moved for " << otherColor << " with board values are:\n";
				for (int i = 0; tempPossibleMoves[i]; i++)
					cout << tempPossibleMoves[i] << " " << scores[i] << endl;
				if (otherColor != origColor) {  // move only the best moves
					int *tpm = tempPossibleMoves;
					int *pm = possibleMoves;
					int *ts = scores;
					while (*tpm) {
						if (*ts++ == highestScore) {
							*pm++ = *tpm;
						}
						tpm++;
					}
					*pm = 0;
					
				}
				if (highestScore > 0) {
						buildNextDepthRequest(rb,b,otherColor,origColor,origMove,depth,possibleMoves);
//						cout << comm.rank << "\tsending request for more work: " << rb<< endl;
						comm.send(0,rb,strlen(rb)+1,TAG_WORK_TO_DO);
				} else { // no valid moved for the other player - we get to go again.
					highestScore = validMoves(b, color, possibleMoves, scores);
					if (highestScore > 0) {
							buildNextDepthRequest(rb,b,otherColor,origColor,origMove,depth,possibleMoves);
							comm.send(0,rb,strlen(rb)+1,TAG_WORK_TO_DO);
					} else { // neither player  has any moves return result
						returnResult(comm,b,origColor,origMove);
					}
				}
			} else {
//				cout << "this is the last depth - return score\n";
				returnResult(comm,b,origColor,origMove);				
			}
		} while (!done);
	}
	
	comm.finalize();
	
	return (EXIT_SUCCESS);
}

