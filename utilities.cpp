/*
 * File:   utilities
 * Author: dan bowker
 * Project: othello
 *
 * utility routines to ease common functions such as convert from board square
 * to array index, print log messages, etc...
 *
 * Created on February 7, 2012, 7:30 PM
 */

#include "othello.h"

int bsToAi(int row, int col) {
	return row * RL + col;
}

void aiToBs(int index, int &row, int &col) {
	col = index % RL;
	row = index / RL;
}

void getScore(char b[BS], int &comp, int &hum) {
	comp = 0;
	hum = 0;
	for (int i = 0; i < BS; i++)
		if (b[i] == C) comp++;
		else if (b[i] == H) hum++;
}
int neighbor(int pos, int dir) {
	switch (dir) {
		case 0: return pos - RL - 1; break;
		case 1: return pos - RL; break;
		case 2: return pos - RL + 1; break;
		case 3: return pos - 1; break;
		case 4: return pos + 1; break;
		case 5: return pos + RL - 1; break;
		case 6: return pos + RL; break;
		case 7: return pos + RL + 1; break;
	}
	return pos;
}

int numDigits(int num, int base) {
	if (num < 0) 
		return 0;
	int result = 1;
	int temp = base;
	while (num > temp) {
		temp *= base;
		result++;
	}
		
	return result;
}
char* iToA(char* p, int num, int base, int numDig = 1) {
	numDig = max(numDig,numDigits(num,base));
	int whichDigit = pow(base,numDig - 1);
	while (num) {
		int digit = num / whichDigit;
		if (digit < 10) {
			*p++ = digit + '0';
		} else {
			*p++ = (digit-10) + 'a';
		}
		num = num - digit * whichDigit;
		whichDigit /= base;
	}
	*p = 0;
	return p;
}