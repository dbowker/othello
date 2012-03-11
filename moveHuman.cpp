/* 
 * File:   moveHuman.cpp
 * Author: dan
 *
 * Created on February 14, 2012, 9:16 PM
 */

#include "othello.h"

using namespace std;

/*
 * 
 */
int moveHuman(char b[BS], int validMoves[BS]) {
	int r;
	int c;
	int hMove = 0;
	bool validMove;
	int* pVM;
	static int humanMoveCount = 0;
	int humanMovesInput[17];
	
	humanMovesInput[0] = 34;
	humanMovesInput[1] = 43;
	humanMovesInput[2] = 64;
	humanMovesInput[3] = 65;
	humanMovesInput[4] = 35;
	humanMovesInput[5] = 56;
	
	humanMovesInput[6] = 34;
	humanMovesInput[7] = 43;
	humanMovesInput[8] = 64;
	humanMovesInput[9] = 65;
	humanMovesInput[10] = 35;
	humanMovesInput[11] = 56;

	humanMovesInput[12] = 34;
	humanMovesInput[13] = 43;
	humanMovesInput[14] = 64;
	humanMovesInput[15] = 65;
	humanMovesInput[16] = 35;
	humanMovesInput[17] = 56;

	do {
		cout << "Your move [r,c] (0 0 to terminate) ";
//		cin >> r >> c;
//		if (r == 0 || c == 0) break;

//		hMove = bsToAi(r,c);
		hMove = humanMovesInput[humanMoveCount++];
		cout << "You have chosen to move to " << hMove << endl;
		validMove = false;
		pVM = validMoves;
		while (*pVM && !validMove) {
			validMove = (*pVM++ == hMove);
		}
		if (!validMove) {
			cout << "\nNot a valid move.\nThe * on the board marks your possible moves\n\n";
			displayBoard(b);
		}
	} while (!validMove);

	squareScore(b,hMove,H,true);
	return hMove;
}
