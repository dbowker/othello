/*
 * File:   utilities
 * Author: dan bowker
 * Project: othello
 *
 * utility routines to ease common functions such as convert from board square
 * to array index, print log messages, etc...
 *
 * Created on February 7, 2012, 8:00 PM
 */

#include "othello.h"

int routeScore(char b[BS], int pos, int dir, char color, bool flip) {
	char oColor = (color == C)? H : C;
	int iTemp;
	if (b[pos] == oColor) {
		iTemp = routeScore(b,neighbor(pos,dir),dir,color,flip);
		if (iTemp) {
			if (flip)
				b[pos] = color;
			return iTemp + getSquareValue(pos);
		} else {
			return 0;
		}
	} else {
		if (b[pos] == color)
			return 1;
		else
			return 0;
	}
}

int squareScore(char b[], int pos, char color, bool flip) {

	int iScore = 0;
	int iTemp = 0;

	int r, c;
	aiToBs(pos,r,c);
	if (r < 1 || c < 1 || c > RL-2 || r > RL - 2)
		return 0;
	
	for (int dir = 0; dir < 8; dir++) {   // loop through each of the 8 possible directions
		iTemp = routeScore(b,neighbor(pos,dir),dir,color,flip);
		if (iTemp > 1)
			iScore += iTemp - 1 + getSquareValue(pos);
	}

	if (iScore > 0 && flip)
		b[pos] = color;

	return iScore;

}