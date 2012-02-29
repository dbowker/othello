/* 
 * File:   main.cpp
 * Author: dan bowker
 * Project: othello
 *
 * Created on February 14, 2012, 7:35 PM
 */

#include "othello.h"

int findOneLevelDeepBestMove(char b[BS],char color){

	int possibleMoves[BS];
	int scores[BS];
	
	return validMoves(b, color, possibleMoves, scores);  // valid moves returns best possible move
	
}