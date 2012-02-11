/*
 * File:   buildWorkRequest
 * Author: dan bowker
 * Project: othello
 *
 * overloaded function
 * signature 1: char* buffer, Board, color, initialColor, depth, array of possible moves
 * signature 2: char* buffer, board_color_initColor, depth, array of possible moves
 *
 * Created on February 10, 2012, 2:30 PM
 */

#include "othello.h"

char* buildWorkRequest(char* buffer, char b[], char color, char origColor, int origMove, int depth, int possibleMove) {
	char *p;
	int i;
	for(i=0;i<MAX_REQUEST_SIZE; i++)
		buffer[i] = '.';

	p = buffer;
	

	for(i = 0; i < BS; i++)
		*p++ = b[i];
	
	*p++ = color;
	*p++ = origColor;

	p = iToA(p,origMove,10,2);  // my version of itoa returns the pos of null byte
	p = iToA(p,depth,10,2);
	
	iToA(p,possibleMove,10,2);
	
	return buffer;
}
