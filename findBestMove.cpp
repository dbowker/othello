/*
 * File:   findBestMove.cpp
 * Author: dan bowker
 * Project: othello
 *
 * Returns an array of possible moves for the color specified,  The array is
 * passed in as pVM  (pointer_ValidMoves)
 *
 * Created on February 8, 2012, 5:10 PM
 */

#include "othello.h"

int findBestMove(char b[BS], char color, bool flip) {

    int bestPosition = 0;

    int possibleMoves[BS];
    int scores[BS];
    int i;

    int valid = validMoves(b, color, possibleMoves, scores);
    for (i = 0; i < BS; i++)
        scores[i] = 0;

    return bestPosition;
}
int findBestMove1(char b[BS], char color, bool flip) {

    int bestPosition = 0;

    int bestMoves[BS];
    int possibleMoves[BS];
    int scores[BS];
    int i;
    int cBM = 0;    // count of best moves

    int best = validMoves(b, color, possibleMoves, scores);

    int numWithBest = 0;
    for (i = 0; possibleMoves[i]; i++) {
        if (scores[i] == best) {
            bestMoves[cBM++] = possibleMoves[i];
        }
    }
    int bestIndex = rand() % cBM;
    bestPosition = bestMoves[bestIndex];
    squareScore(b, bestPosition, color, flip);
    return bestPosition;
}
