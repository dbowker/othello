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

void getScore(char b[BS], int &comp, int &hum) {
    comp = 0;
    hum = 0;
    for (int i = 0; i < BS; i++)
        if (b[i] == C) comp++;
        else if (b[i] == H) hum++;
}
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