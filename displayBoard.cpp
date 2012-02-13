/*
 * File:   displayBoard
 * Author: dan bowker
 * Project: othello
 *
 * Display board
 *
 * Created on February 7, 2012, 7:30 PM
 */

#include "othello.h"

void displayBoardWithValues(char b[BS], char color) {
	int i,j;
	int comp, hum;
	int value;

	cout << "\n   ";
	for (j = 0; j < RL-0; j++)   // 1 1
		cout << "  " << j << "  ";
	
	cout << endl << "   ";
	
	for (j = 0; j < RL-0; j++)   // 1 2
		cout << "----|";
	cout << "----\n";

	for (i = 0; i < RL-0; i++) {  // 1 1
		cout << i << " |";
		for (j = 0; j < RL-0; j++) { // 1 1
			if (b[bsToAi(i,j)] != ' ')
				cout << " " << b[bsToAi(i,j)] << "  |";
			else {
				value = squareScore(b,bsToAi(i,j),color,false);
//				cout << "value [" << i << "," << j << "] = " << value << endl;
				if (value)
					cout << " " << setw(2) << value << " |";
				else
					cout << " " << "  " << " |";
			}
		}
		cout << endl << "   ";
		for (j = 0; j < RL-0; j++)  // 1 2
			cout << "----|";
		cout << "----\n";
	}

	getScore(b,comp,hum,false);
	cout << "Score:  Computer: " << comp << "   Human: " << hum << "\n\n";

}

void displayBoard(char b[BS], char color) {
	int i,j;
	int comp, hum;
	int value;

	cout << "\n   ";
	for (j = 1; j < RL-1; j++)
		cout << "  " << j << " ";
	
	cout << endl << "   ";
	
	for (j = 1; j < RL-1; j++)   // 1 2
		cout << "---|";
	cout << "---\n";

	for (i = 1; i < RL-1; i++) {  // 1 1
		cout << i << " |";
		for (j = 1; j < RL-1; j++) { // 1 1
			if (b[bsToAi(i,j)] != ' ')
				cout << " " << b[bsToAi(i,j)] << " |";
			else {
				value = squareScore(b,bsToAi(i,j),color,false);
//				cout << "value [" << i << "," << j << "] = " << value << endl;
				if (value)
					cout << " " << "*" << " |";
				else
					cout << " " << " " << " |";
			}
		}
		cout << endl << "   ";
		for (j = 1; j < RL-1; j++)  // 1 2
			cout << "---|";
		cout << "---\n";
	}

	getScore(b,comp,hum,false);
	cout << "Score:  Computer: " << comp << "   Human: " << hum << "\n\n";

}

