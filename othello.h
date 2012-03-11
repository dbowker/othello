/*
 * File:   othello.h
 * Author: dan
 *
 * Created on February 7, 2012, 7:13 PM
 */

#ifndef _OTHELLO_H
#define	_OTHELLO_H

#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <cstdlib>
#include <limits.h>
#include <cmath>
#include <stack>
#include <string.h>
#include <list>
#include <fstream>
#include "Communicator.h"

using namespace std;
const int PA = 8;			// play area
const int RL = (PA + 2);	// Row length of the board
const int BS = RL * RL;		// board size is a square 8 plus padding
const int MAX_POSSIBLE = BS / 3;

const char C = 'X';		 // Computer's marker
const char H = 'O';		 // Human's marker

const int MAX_DEPTH = 10;
const int TAG_TO_QUEUE = 0;
const int TAG_TERMINATE = 1;
const int TAG_PROC_AVAILABLE = 2;
const int TAG_DO_THIS_WORK = 3;
const int TAG_RESULT = 4;

const int NORMAL_PIECE = 1;
const int EDGE_PIECE = 25;
const int UNFLIPPABLE_PIECE = 500;

// data passing structures

class WorkRequest {
public:
	WorkRequest(){}

	char b[BS];						// 100
	char color;						//   1
	char origColor;					//   1
	char depth;						//   1
	char history[MAX_DEPTH*2+1];	//	21
	int  scores[MAX_DEPTH*2+1];		//  84
};                                  // 208 <-- total byte count

class WorkAddition {
public:
	char b[BS];						// 100
	char color;						//   1
	char origColor;					//   1
	char depth;						//   1
	char history[MAX_DEPTH*2+1];	//  21
	int  scores[MAX_DEPTH*2+1];		//  84
	char additionalMoves[PA*PA / 2];//  32
	int  processorTime;				//   4
};									// 244 <-- total byte count

class WorkResult {
public:
	WorkResult(){}
	int  boardValue;				//	4
	char history;					//  1
	int  processorTime;				//  4
};									//  9 <-- total byte count


// prototypes

int bsToAi(int, int);
void aiToBs(int, int &, int &);
void getScore(char [], int &, int &, bool weighted=true);
int neighbor(int, int);
WorkResult* makeResult(WorkResult*, WorkRequest*);

void setupBoard(char []);
void displayBoard(char [], char color = H);
//void displayBoardWithValues(int []);
int validMoves(char [], char, int *, int *);
int squareScore(char [], int, char, bool);
int getSquareValue(int);
int getBoardValue(char [],  char);
int findBestMove(Communicator, char [], char , int);
void returnResult(Communicator, char[], char, int, int*);
int findOneLevelDeepBestMove(char [],char );
int moveHuman(char [], int []);

void makeWorkRequest(WorkRequest*, WorkAddition*, int);
void makeWorkAddition(WorkAddition*, WorkRequest*, int[]);
void processRequest(Communicator, WorkRequest*, int &, int [], int []);
void worker(Communicator);
void logIt(char*);
void logIt(long);
int elapsedTime(struct timeval *, struct timeval *);
#endif	/* _OTHELLO_H */
