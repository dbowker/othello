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

void displayBoard(char b[BS]) {
    int i,j;
    int comp, hum;

    cout << "\n   ";
    for (j = 1; j < RL-1; j++)
        cout << " " << j << "  ";
    
    cout << endl << "   ";
    
    for (j = 1; j < RL-2; j++) 
        cout << "---|";
    cout << "---\n";

    for (i = 1; i < RL-1; i++) {
        cout << i << " |";
        for (j = 1; j < RL-1; j++) {
            if (b[bsToAi(i,j)] != ' ')
                cout << " " << b[bsToAi(i,j)] << " |";
            else
                if (squareScore(b,bsToAi(i,j),H,false))
                    cout << " " << "*" << " |";
                else
                    cout << " " << " " << " |";
        }
        cout << endl << "   ";
        for (j = 1; j < RL-2; j++)
            cout << "---|";
        cout << "---\n";
    }

    getScore(b,comp,hum);
    cout << "Score:  Computer: " << comp << "   Human: " << hum << "\n\n";

}


