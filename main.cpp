/* 
 * File:   main.cpp
 * Author: dan bowker
 * Project: othello
 *
 * Created on February 7, 2012, 7:12 PM
 */

#include "othello.h"

/*
 * 
 */
int main(int argc, char** argv) {

    char b[BS];
    int r, c;
    int validHMoves[BS];
    int validCMoves[BS];
    int scoreHMoves[BS];
    int scoreCMoves[BS];
    int *pVM;
    int hMove;
    bool validMove;

    srand((int) time(NULL));
    setupBoard(b);

    do {
        displayBoard(b);
        validMoves(b,H,validHMoves,scoreHMoves);
        validMoves(b,C,validCMoves,scoreCMoves);
        if (validHMoves[0]) {
            do {
                cout << "Your move [r,c]";
                cin >> r >> c;
                hMove = bsToAi(r,c);
                validMove = false;
                pVM = validHMoves;
                while (*pVM && !validMove) {
                    validMove = (*pVM++ == hMove);
                }
                if (!validMove)
                    cout << "\nNot a valid move.\nThe * on the board marks your possible moves\n\n";
                else
                    squareScore(b,hMove,H,true);
                int best = findBestMove(b,C,true);
                aiToBs(best, r, c);
                cout << "Computer moves to [" << r << "," << c << "]\n";
            } while (!validMove);
        }
    } while (validCMoves[0] || validHMoves[0]);

    cout << "\n\nNO more valid moves\n\n";

    return (EXIT_SUCCESS);
}

