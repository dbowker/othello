/* 
 * File:   main.cpp
 * Author: dan bowker
 * Project: othello
 *
 * Created on February 14, 2012, 7:35 PM
 */

#include "othello.h"

int findOneLevelDeepBestMove(char b[BS],char color){

	char tB[BS];
	int possibleMoves[BS];
	int scores[BS];
	
	validMoves(b, color, possibleMoves, scores);
	
//	cout << "valid moved for " << color << " with board values are:\n";
//	cout << "highest possible move is " << highestPossible << endl;

	for (int i = 0; possibleMoves[i]; i++) {
//		cout << possibleMoves[i] << " " << scores[i] << endl;
//		if (scores[i] == highestPossible)
			return possibleMoves[i];
	}
	return 0;
}