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
#include <ctime>
#include <cstdlib>
#include <cmath>

using namespace std;

const int RL = (8 + 2);     // Row length of the board 
const int BS = RL * RL;     // board size is a square 8 plus padding

const char C = 'X';         // Computer's marker
const char H = 'O';         // Human's marker

// prototypes

int bsToAi(int, int);
void aiToBs(int, int &, int *);
void getScore(char [], int &, int &);
int neighbor(int, int);

void setupBoard(char b[]);
void displayBoard(char b[]);

int squareScore(char b[], int pos, char color, bool flip);

#endif	/* _OTHELLO_H */

