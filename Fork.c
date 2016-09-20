/* being Fork.c */
/*
 * Forks and Pipes
 */
#include "board.h"
#include "csapp.h"
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

void sigchld_handler(int sig);
void playGame(Board brd);

int main(int argc, char *argv[]) 
{
	if(argc != 3){
		/* Daniel Day-Lewis */
		perror("You see this knife, I'm gonna teach you to speak English with this fucking knife");
		exit(0);
	}

	/* board size and number of games */
	int brdSize = atoi(argv[1]);
	int numGames = atoi(argv[2]);
	int i; /* incrementor */
	
	Board brdGames[numGames];
	int pipefd[numGames*2];

	Signal(SIGCHLD, sigchld_handler);
	for(i=0; i<numGames; i++) {
		//atexit(cleanBoard(brdGames[i]));
		printf("Game\n");
		pipe(pipefd);
		pipe(pipefd);
		pid_t pid = Fork();
		if(pid == 0) {
			brdGames[i] = createSqBoard(brdSize);
			playGame(brdGames[i]);
			printf("\n");
		}
	}
	return 0;
}

/*
 * sigchld_handler: for child signals
 * @param sig: signal
 * no return
 */
void sigchld_handler(int sig) {
	while(waitpid(-1, 0, WNOHANG) > 0) ;
	return;
}

/*
 * playGame: plays the game, determine the winner
 * @param brd: the board
 * no return
 */
void playGame(Board brd) {
	const int RED = 1; /* RED is represented by 1 */
	const int BLUE = 2; /* BLUE is represented by 2 */
	int i = 0;
	int gm;
	while(!(gm = gameOver(brd))) /* while the game isn't over */
	{
		/* RED Moves */
		if(i%2 == 1) {
			brd = randomMove(brd, RED);
		}
		/*BLUE Moves */
		else {
			brd = randomMove(brd, BLUE);
		}
		i++;
	}

	printBoard(brd);
	switch(gm) {
		case 1:
			printf("RED wins!\n");
			break;
		case 2:
			printf("BLUE wins!\n");
			break;
		case 3:
			printf("Everyone loses\n");
			break;
		default:
			/* curb your enthusiasm */
			perror("Somebody gotta get fucked up, it ain't gonna be me");
			break; /* I never understood why this break was necessary */
	}
}
