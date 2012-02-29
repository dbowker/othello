/*
 * File:   validMoves.cpp
 * Author: dan bowker
 * Project: othello
 *
 * Returns an array of possible moves for the color specified,  The array is
 * passed in as pVM  (pointer_ValidMoves), also return the possible scores for
 * each move
 * 
 * the function's returned value is the position with the best move.
 *
 * Created on February 8, 2012, 4:55 PM
 */

#include "othello.h"

int validMoves(char b[BS], char color, int *pVM, int *pS) {

	int pos;
	int score;
	int highestScore = -LONG_MAX;
	int bestPos = 0;
	*pVM = 0;	   // zero is the sentinel marking the end of valid moves,
	char tB[BS];
	score = -LONG_MAX;
	
	for(int r = 1; r < RL-1; r++) {
		for (int c = 1; c < RL-1; c++) {
			pos = bsToAi(r,c);
			if (b[pos] == ' ') {
				score = squareScore(b, pos, color, false);
				if (score) {
					strncpy(tB,b,BS);
					squareScore(tB,pos,color,true);
					score = getBoardValue(tB,color);
//					if (color == H) {
//						cout << "for color " << color << " just checked " << pos << " score = " << score << endl;
//					}
					*pVM++ = pos;
					*pS++ = score;
					*pVM = 0;
					if (score > highestScore) {
						highestScore = score;
						bestPos = pos;
					}
				}
			}
		}
	}
	return bestPos;  // if highest is 0 no valid moves exist
}
