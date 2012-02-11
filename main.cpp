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
			int possibleMoves[BS];
			int scores[BS];

			comm.recv(0, &rb[0], MAX_REQUEST_SIZE, TAG_DO_THIS_WORK);
			char color;
			char origColor;
			int  origMove;
			int  depth;
			int  move;
			parseRequest(rb, b, color, origColor, origMove, depth, move);
			cout << comm.rank << "\treceived this request  color:"<< color << " origColor:"<< origColor << "  origMove:" << origMove << "  depth:" << depth << "  move:" << move << endl;;
			int tc,th;
			getScore(b,tc,th);
			cout << "score before flipping: H:" << th << " C:" << tc << endl;
			squareScore(b,move,color,true);
			cout << "score after  flipping: H:" << th << " C:" << tc << endl;
			if (--depth) {
				int numPossible = validMoves(b, color, possibleMoves, scores);
//				cout << comm.rank << "\tNumber of possible moves: " << numPossible << endl;
				if (numPossible > 0) {
					for (int i=0; possibleMoves[i]; i++) {
						scores[possibleMoves[i]] = 0;
						buildWorkRequest(rb,b,color,origColor,origMove,depth,possibleMoves[i]);
						comm.send(0,rb,strlen(rb)+1,TAG_WORK_TO_DO);
					}
				} else { // no valid moved for this player - try other
					char otherColor = (color==C)?H:C;
					numPossible = validMoves(b, otherColor, possibleMoves, scores);
					if (numPossible > 0) {
						for (int i=0; possibleMoves[i]; i++) {
							scores[possibleMoves[i]] = 0;
							buildWorkRequest(rb,b,otherColor,origColor,origMove,depth,possibleMoves[i]);
							comm.send(0,rb,strlen(rb)+1,TAG_WORK_TO_DO);
						}
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

