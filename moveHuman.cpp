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
	
	do {
		cout << "Your move [r,c]";
		cin >> r >> c;
		if (r == 0 || c == 0) break;

		hMove = bsToAi(r,c);
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
