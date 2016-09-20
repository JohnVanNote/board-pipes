/* begin board.h */
#ifndef _BOARD_H_
#define _BOARD_H_

#include <stddef.h>

/* Board structure */
typedef struct {
	size_t length;
	size_t height;
	int **spots;

} Board;

Board createBoard(size_t l, size_t h);
Board createSqBoard(size_t s);
Board copyBoard(Board brd);
Board mutateBoard(Board brd, int x, int y, int val);
void cleanBoard(Board *brd);

int validMove(Board brd, int x, int y);
Board randomMove(Board brd, int val);
int **findMoves(Board brd, int *pm);
int gameOver(Board brd);

void printBoard(Board brd);
#endif
/* end board.h*/
