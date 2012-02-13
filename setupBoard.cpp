/*
 * File:   setupBoard
 * Author: dan bowker
 * Project: othello
 *
 * setup the initial state of the board.
 *
 * Created on February 7, 2012, 7:30 PM
 */

#include "othello.h"

void setupBoard(char b[BS]) {
	int i;

	for (i = 0; i < BS; i++)
		b[i] = ' ';
/*
	b[bsToAi(4,4)] = C;
	b[bsToAi(5,5)] = C;

	b[bsToAi(4,5)] = H;
	b[bsToAi(5,4)] = H;
*/

	b[34] = H;
	b[41] = C;
	b[42] = H;
	b[44] = H;
	b[45] = H;
	b[51] = C;
	b[52] = H;
	b[53] = H;
	b[54] = H;
	b[55] = H;
	b[61] = C;
	b[62] = H;
	b[64] = H;
	b[65] = H;
	b[72] = H;
	b[82] = H;

			
//	               xppppppppp112345678p212345678p312345678p412345678p512345678p612345678p712345678p812345678pxppppppppx
//		strncpy(b,"                                  O      XO OO     XOOOO     XO OO      O         O                 ",BS);

}

