/*
 * File:   SendRequst.cpp
 * Author: dan bowker
 * Project: othello
 *
 * Returns an array of possible moves for the color specified,  The array is
 * passed in as pVM  (pointer_ValidMoves)
 *
 * Created on February 10, 2012, 3:10 PM
 */

#include "othello.h"

void sendRequest(Communicator comm, char* buffer, int to) {
	
	comm.send(to, buffer, strlen(buffer), TAG_DO_THIS_WORK);
	
}
