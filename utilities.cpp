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
	return 1;
}

int isRail(int pos) {
	int r, c;
	aiToBs(pos, r, c);
	return (r < 1 || c < 1 || r > PA || c > PA);
}

int getBoardValue(char b[BS], char color) {
	static int bv[BS];
	int cScore = 0;
	int hScore = 0;
	int i, r, c;
	int result = 0;
	int tC = ' ';
	int pos;
	char cc;
	// fill in all the "normal" square values
	for (i = 0; i < BS; i++) 
		bv[i] = (b[i] != ' ') ? NORMAL_PIECE : 0;

	// top left
	pos = 1;
	cc = b[bsToAi(1,pos)];		// corner color
	for (r=1; cc != ' ' && b[bsToAi(r,1)] == cc; r++) {
		while (b[bsToAi(r,pos)] == cc)
			bv[bsToAi(r,pos++)] = UNFLIPPABLE_PIECE;
		pos = 1;
	}
	
	// top right
	pos = PA;
	cc = b[bsToAi(1,pos)];		// corner color
	for (r=1; cc != ' ' && b[bsToAi(r,PA)] == cc; r++) {
		while (b[bsToAi(r,pos)] == cc)
			bv[bsToAi(r,pos--)] = UNFLIPPABLE_PIECE;
		pos = PA;
	}

	// bottom left
	pos = 1;
	cc = b[bsToAi(PA,pos)];		// corner color
	for (r=PA; cc != ' ' && b[bsToAi(r,1)] == cc; r--) {
		while (b[bsToAi(r,pos)] == cc)
			bv[bsToAi(r,pos++)] = UNFLIPPABLE_PIECE;
		pos = 1;
	}


	// bottom right
	pos = PA;
	cc = b[bsToAi(PA,pos)];		// corner color
	for (r=PA; cc != ' ' && b[bsToAi(r,PA)] == cc; r--) {
		while (b[bsToAi(r,pos)] == cc)
			bv[bsToAi(r,pos--)] = UNFLIPPABLE_PIECE;
		pos = PA;
	}
	for(i=0; i < BS; i++ ) {
		if (b[i] == C) cScore += bv[i];
		if (b[i] == H) hScore += bv[i];
	}
	displayBoardWithValues(bv);
	return (color=C) ? cScore - hScore : hScore - cScore;
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

WorkResult* makeResult(WorkRequest* in) {
	WorkResult* out = new WorkResult();
	int i = 0;
	for (i = 0; in->history[i];  i++) {
		out->history[i] = in->history[i];
		out->scores[i] = in->scores[i];
	}
	out->history[i] = 0;
	out->boardValue = in->scores[i-1];
	return out;
}