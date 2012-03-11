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

//int isRail(int pos) {
//	int r, c;
//	aiToBs(pos, r, c);
//	return (r < 1 || c < 1 || r > PA || c > PA);
//}

// getBoardValue 
// each square on the board is worth 1 point if there is on a chip on it
// unless a chip can't be flipped
// unflippable is determined if it's not possible for the piece to ever be
// surrounded.  
int getBoardValue(char b[BS], char color) {
	static int bv[BS];
	int cScore = 0;
	int hScore = 0;
	int i, r, c;
	int pos;
	char cc;

	// fill in all the "normal" square values
	for (i = 0; i < BS; i++) 
		bv[i] = (b[i] != ' ') ? NORMAL_PIECE : 0;

	for (c = 1; c <= PA; c++) {
		r = 1;
		bv[bsToAi(r,c)] = (b[bsToAi(r,c)] != ' ') ? EDGE_PIECE : 0;
		bv[bsToAi(c,r)] = (b[bsToAi(c,r)] != ' ') ? EDGE_PIECE : 0;
		r = PA;
		bv[bsToAi(r,c)] = (b[bsToAi(r,c)] != ' ') ? EDGE_PIECE : 0;
		bv[bsToAi(c,r)] = (b[bsToAi(c,r)] != ' ') ? EDGE_PIECE : 0;
		
	}
	// from each corner determine if the chip can be flipped
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
	
	// determine total board value for each color
	for(i=0; i < BS; i++ ) {
		if (b[i] == C) cScore += bv[i];
		if (b[i] == H) hScore += bv[i];
	}
	
	// return the relative score for the color desired
	return (color==C) ? cScore - hScore : hScore - cScore;
}
//
//  return the chip count for each color
//
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

// which square is to the .... upper left, straight up...
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

WorkResult* makeResult(WorkResult* out, WorkRequest* in) {
	int boardValue = 0;
	int i = 0;
//	cout << "Making result\n";
	for (i = 0; in->history[i];  i++) {			// loop through history and weight each historical score
//		cout << (short) in->history[i] << " (" << (short)in->scores[i] << ")\n";
		boardValue += (double)in->scores[i] / (double)((i+1)*(i+1));
	}
	out->history = in->history[0];				// original move is in history[0]
	out->boardValue = boardValue;				// just computed value
	return out;
}

void makeWorkAddition(WorkAddition* wAdd, WorkRequest* wReq, int moves[]) {
	int i;
	strncpy(wAdd->b,wReq->b,BS);
	wAdd->color = wReq->color;
	wAdd->origColor = wReq->origColor;
	wAdd->depth = wReq->depth;
	for(i=0; wReq->history[i]; i++) {
		wAdd->history[i] = wReq->history[i];
		wAdd->scores[i] = wReq->scores[i];
	}
	wAdd->history[i] = 0;
	for(i=0; moves[i]; i++)
		wAdd->additionalMoves[i] = moves[i];
	wAdd->additionalMoves[i] = 0;
}

void makeWorkRequest(WorkRequest* wReq, WorkAddition* wAdd, int move) {
	int i;
	strncpy(wReq->b,wAdd->b,BS);
	wReq->color = wAdd->color;
	wReq->origColor = wAdd->origColor;
	wReq->depth = wAdd->depth;
	for(i=0; wAdd->history[i]; i++) {
		wReq->history[i] = wAdd->history[i];
		wReq->scores[i] = wAdd->scores[i];
	}
	wReq->history[i++] = move;
	wReq->history[i] = 0;
}

void logIt(long num) {
	char buf[50];
	sprintf(buf,"%9.2f",(float)num/1000000);
//	sprintf(buf,"%d",num);
	logIt(buf);
}

void logIt(char* text) {
	static ofstream outputFile;
	if (!outputFile.is_open()) {
		outputFile.open("othello.log",ios::app);
	}
	outputFile << text << "\n";
	cout << text << endl;
	
	outputFile.flush();
}

int elapsedTime(struct timeval *t2, struct timeval *t1) {

    return  (t2->tv_usec + 1000000 * t2->tv_sec) - (t1->tv_usec + 1000000 * t1->tv_sec);
   
}
