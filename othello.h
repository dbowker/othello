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
#include <cmath>
#include <stack>
#include <string.h>
#include "Communicator.h"

using namespace std;
const int PA = 8;			// play area
const int RL = (PA + 2);	// Row length of the board 
const int BS = RL * RL;		// board size is a square 8 plus padding
const int MAX_POSSIBLE = BS / 3;

const char C = 'X';		 // Computer's marker
const char H = 'O';		 // Human's marker

const int TAG_TO_QUEUE = 0;
const int TAG_TERMINATE = 1;
const int TAG_PROC_AVAILABLE = 2;
const int TAG_DO_THIS_WORK = 3;
const int TAG_RESULT = 4;

const int NORMAL_PIECE = 1;
const int UNFLIPPABLE_PIECE = 99;

// data passing structures

class WorkRequest {
public:
	WorkRequest(){}

	char b[BS];
	char color;
	char origColor;
	int depth;
	int history[PA*PA/2];
	int scores[PA*PA/2];
};

class WorkResult {
public:
	WorkResult(){}
	int boardValue;
	int history;//[PA*PA/2];
//	int scores[PA*PA/2];
};

// prototypes

int bsToAi(int, int);
void aiToBs(int, int &, int &);
void getScore(char [], int &, int &, bool weighted=true);
int neighbor(int, int);
char* iToA(char* , int, int, int);
int aToI(char* &, int, int);
WorkResult* makeResult(WorkResult*, WorkRequest*);

void setupBoard(char []);
void displayBoard(char [], char color = H);
void displayBoardWithValues(int []);
int validMoves(char [], char, int *, int *);
int squareScore(char [], int, char, bool);
int getSquareValue(int);
int getBoardValue(char [],  char);
int findBestMove(Communicator, char [], char , int );
//char* buildWorkRequest(char*, char [], char, char,int, int, int, int*);
//char* buildNextDepthRequest(char* rb, char[], char, char, int, int , int*, int*);
//void parseRequest(char*, char [], char &, char &, int &, int &, int &, int*);
//void parseNextDepthRequest(char*, char [], char &, char &, int &, int &, int *, int*);
void returnResult(Communicator, char[], char, int, int*);
int findOneLevelDeepBestMove(char [],char );
int moveHuman(char [], int []);

#endif	/* _OTHELLO_H */
