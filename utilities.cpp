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
int getSquareValue(int pos) {
	const int MIDDLE_PIECE = 1;
	const int EDGE_PIECE = 8;
	const int CORNER_PIECE = 99;
	int r, c;
	int value = 1;
	aiToBs(pos,r,c);
	if (r == 1 || r == RL-2) value += EDGE_PIECE;  // edge piece
	if (c == 1 || c == RL-2) value += EDGE_PIECE;  // edge piece
	if (value == MIDDLE_PIECE + 2*EDGE_PIECE) value = CORNER_PIECE;			  // corner piece
	return value;
}

void getScore(char b[BS], int &comp, int &hum, bool weighted) {
	comp = 0;
	hum = 0;
	int value = 1;
	for (int i = 0; i < BS; i++) {
		if (b[i] != ' ') {
			value = (weighted) ? getSquareValue(i) : 1;
			if (b[i] == C) comp += value;
			else if (b[i] == H) hum += value;
		}
	}
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
	char* origP = p;
	int   origNum = num;
	numDig = max(numDig,numDigits(num,base));
	if (num < 0) {
		*p++ = '-';
		num *= -1;
	}
	int whichDigit = pow(base,numDig - 1);
	while (numDig--) {
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

int aToI(char* &p, int digits, int base) {
	int result = 0;
	bool negative = false;
	if (*p == '-') {
		p++;
		negative = true;
	} else if (*p == '+') {
		p++;
	}
	while (digits-- > 0) {
		if (*p <= '9') {
			result = result*base + (*p++ - '0');
		} else {
			if (*p >= 'a') {
				result = result*base + (*p++ - 'a');
			} else {
				result = result*base + (*p++ - 'A');
			}
		}
	}
	return result;
}