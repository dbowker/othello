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
#include "WorkQueue.h"

using namespace std;

const int RL = (8 + 2);	 // Row length of the board 
const int BS = RL * RL;	 // board size is a square 8 plus padding
const int MOVE_DIGITS = (int)log10(BS);
const int MAX_POSSIBLE = BS / 3;
const int MAX_REQUEST_SIZE = BS + 1 + 1 + MOVE_DIGITS + MAX_POSSIBLE*MOVE_DIGITS;

const char C = 'X';		 // Computer's marker
const char H = 'O';		 // Human's marker

const int TAG_WORK_TO_DO = 0;
const int TAG_TERMINATE = 1;
const int TAG_DO_THIS_WORK = 3;
const int TAG_RESULT = 4;

// prototypes

int bsToAi(int, int);
void aiToBs(int, int &, int &);
void getScore(char [], int &, int &, bool weighted=true);
int neighbor(int, int);
char* iToA(char* , int, int, int);

int aToI(char* &, int, int);

void setupBoard(char []);
void displayBoard(char [], char color = H);
int validMoves(char [], char, int *, int *);
int squareScore(char [], int, char, bool);
int getSquareValue(int);
int findBestMove(Communicator, char [], char , int );
char* buildWorkRequest(char*, char [], char, char,int, int, int);
char* buildNextDepthRequest(char* rb, char[], char, char, int, int depth,int []);
void parseRequest(char*, char [], char &, char &,int &, int &,int &);
void returnResult(Communicator, char[], char, int);
void parseRequest(char*, char [], char &, char &, int &, int &, int*);


#endif	/* _OTHELLO_H */

