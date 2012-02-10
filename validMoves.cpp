/*
 * File:   validMoves.cpp
 * Author: dan bowker
 * Project: othello
 *
 * Returns an array of possible moves for the color specified,  The array is
 * passed in as pVM  (pointer_ValidMoves), also return the possible scores for
 * each move
 *
 * Created on February 8, 2012, 4:55 PM
 */

#include "othello.h"

int validMoves(char b[BS], char color, int *pVM, int *pS) {

    int pos;
    int score;
    int highest = 0;
    *pVM = 0;       // zero is the sentinal marking the end of valid moves,

    for(int r = 0; r < RL; r++) {
        for (int c = 0; c < RL; c++) {
            pos = bsToAi(r,c);
            if (score = squareScore(b, pos, color, false)) {
                *pVM++ = pos;
                *pS++ = score;
                *pVM = 0;
                if (score > highest)
                   highest = score;
            }
        }
    }
    return highest;
}
