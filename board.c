/* begin board.c */
#include "board.h"
#include "csapp.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stddef.h>

/*
 * createBoard: creates a blank Board (all Open Spaces)
 * @param l: length of Board (must be larger than 3)
 * @param h: height of Board (must be larger than 3)
 * @return: a blank Board
 */
Board createBoard(size_t l, size_t h) {
	Board Brd;
	int i,j; /* incrementors */
	assert(l>3); /* so 4-in-a-row is possible */
	assert(h>3); /* so 4-in-a-row is possible */
	Brd.length = l;
	Brd.height = h;

	/* initializes 2-D array */
	int **emptySpots = (int**)Malloc(sizeof(int*) * l);
	for(i=0; i<h; i++) {
		emptySpots[i] = (int*)Malloc(sizeof(int)*h);
	}

	for(i=0; i<l; i++) {
		for(j=0; j<h; j++) {
			emptySpots[i][j] = 0;
		}
	}
	Brd.spots = emptySpots;
	return Brd;
}

/*
 * createSqBoard: creates a square board
 * @param s: the length and height of the board
 * @return: what you think it returns
 */
Board createSqBoard(size_t s) {
	Board Brd = createBoard(s,s);
	return Brd;
}

/*
 * copyBoard: copies an existing Board
 * @param brd: the Board that is to be copied
 * @return: a copy of the board
 */
Board copyBoard(Board brd) {
	Board cpyBrd;
	int i,j; /* incrementors*/
	int l = brd.length;
	int h = brd.height;

	/* 2-D array thing */
	int **cpySpots = (int**)Malloc(sizeof(int*) * l);
	for(i=0; i<h; i++) {
		cpySpots[i] = (int*)Malloc(sizeof(int)*h);
	}

	/* copy, copy, copy */
	for(i=0; i<l; i++) {
		for(j=0; j<h; j++) {
			cpySpots[i][j] = brd.spots[i][j];
		}
	}

	cpyBrd.length = l;
	cpyBrd.height = h;
	cpyBrd.spots = cpySpots;
	return cpyBrd;
}

/*
 * mutateBoard: takes a Board and attempts to modify it
 * @param brd: the Board to be modified
 * @param x: the potential horizontal value to be changed
 * @param y: the potential vertical value to be changed
 * @param val: the val (either 1 or 2) that goes in (x,y)
 * @return: the modified Board
 */
Board mutateBoard(Board brd, int x, int y, int val) {
	/* You take the red pill - you stay in wonderland
		 and i'll show you how deep the rabbit hole goes */
	if((val != 1) && (val != 2)) {
		perror("MissingNo.\n");
		perror("Surf along Cinnabar Island\n");
		exit(0);
	}

	/* checks to see if the move is valid */
	if(validMove(brd,x,y)) {
		brd.spots[x][y] = val;
	}
	else {
		perror("Invalid move");
	}
	return brd;
}

/*
 * cleanBoard: frees board memory
 * @param brd; a pointer to the Board
 * no return
 */
void cleanBoard(Board *brd) {
	int i;
	int l = brd->length;
	int h = brd->height;
	for(i=0; i<h; i++) {
		Free((*brd).spots[i]);
	}
	Free((*brd).spots);
}

/*
 * validMove: a "private" function to validate a move of the Board
 * @param brd: the Board in question
 * @param x: the x(horizontal) board location
 * @param y: the y(veritcal) Board location l  
 * return: 1 if move is legal, 0 otherwise
 */
int validMove(Board brd, int x, int y) {
	int i; /* incrementor */
	int l = brd.length;
	int h = brd.height;
	/* checks if spot is on board */
	if((x<0) || (y<0))
		return 0;
	if((x>=l) || (y>=h))
		return 0;
	/* checks to see there are no floating pieces */
	if(y >= 1)
		for(i=y-1; i==0; i--)
			if(brd.spots[x][y] == 0)
				return 0;
	return 1;
}

/*
 * randomMove: finds validMoves picks a random one
 * @param val: 1 or 2, 1 for RED, 2 for BLUE
 * @return the board with a random move
 */
Board randomMove(Board brd, int val) {
	int i; /* incrementor */
	int pm = 0;
	int **moves = findMoves(brd, &pm);
	int r = random() % pm;
	int x = moves[r][0];
	int y = moves[r][1];
	brd = mutateBoard(brd, x, y, val);
	
	/* clean up */
	for(i=0; i<2; i++)
		Free(moves[i]);
	Free(moves);
	return brd;
}

/*
 * findMoves: finds all valid moves
 * @param brd: the Board in question
 * @pm: should be 0, will return the NUMBER of possible moves
 * return: an array of an array of ints of possible moves
 */
int **findMoves(Board brd, int *pm) {
	int l = brd.length;
	int h = brd.height;
	int i,j; /* incrementors */

	/* possibles moves */
	int **possMoves = (int**)Malloc(sizeof(int*)*l*h);
	for(i=0; i<l; i++) {
		possMoves[i] = (int*)Malloc(sizeof(int)*2);
	}

	for(i=0; i<l; i++) {
		for(j=0; j<h; j++) {
			if(validMove(brd,i,j)) {
				possMoves[(*pm)][0] = i;
				possMoves[(*pm)][1] = j;
				(*pm)++;
			}
		}
	}
	return possMoves;
}

/*
 * gameOver: tells if the game is over
 * @param brd: the Board
 * @return: 0 if game is not over, 
 *          1 if RED wins, 
 *          2 if BLUE wins, 
 *          3 if tied
 */
int gameOver(Board brd) {
	const int TIED = 3;
	int l = brd.length;
	int h = brd.height;
	int **gameSpots = brd.spots;
	int i, j;
	int v1, v2, v3, v4;
	for(i=0; i<l-3; i++) {
		for(j=0; j<h-3; j++) {
			if(gameSpots[i][j] == 0)
				break;
			/* check verticle */
			v1 = gameSpots[i][j];
			v2 = gameSpots[i][j+1];
			v3 = gameSpots[i][j+2];
			v4 = gameSpots[i][j+3];
			if((v1 == v2) == (v3 == v4))
				return v1;
			/* checks horizontal */
			v1 = gameSpots[i][j];
			v2 = gameSpots[i+1][j];
			v3 = gameSpots[i+2][j];
			v4 = gameSpots[i+3][j];
			if((v1 == v2) == (v3 == v4))
				return v1;
			/* horizontal checking to be implemented later */
		}
	}
	return TIED;
}

/*
 * printBoard: prints Board to stdout
 * @param brd: the Board
 * no return
 */
void printBoard(Board brd) {
	int i,j,spot;
	int l = brd.length;
	int h = brd.height;
	for(i=l-1; i>=0; i--) {
		for(j=0; j<h; j++) {
			spot = brd.spots[j][i];
			switch(spot) {
				case 0:
					printf("open ");
					break;
				case 1:
					printf("RED  ");
					break;
				case 2:
					printf("BLUE ");
					break;
				default:
					perror("printing error...\n");
					return;
			}
		}
		printf("\n");
	}
}
/* end board.c */
