/* 
 * Ather Fawaz
 * OS Section C
 * 17-4004
 *
 * Conway's Game of Life
 *
 * A cell is born, if it has exactly three neighbours
 * A cell dies of loneliness, if it has less than two neighbours
 * A cell dies of overcrowding, if it has more than three neighbours
 * A cell survives to the next generation, if it does not die of loneliness or overcrowding
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <time.h>
#include "Barrier.h"
#include "Cells.h"

//--Preprocessor Directives--//
#define BOARD_HEIGHT 25
#define BOARD_WIDTH 25
#define CLEAR_SCREEN() puts ("\033[H\033[J")
#define STATISTICS generations, _cells.alive, _cells.maintained, _cells.died
//--Preprocessor Directives--//

//--Global Variables--//
long int generations = 0;				//First generation is the zeroth generation
int board[BOARD_HEIGHT][BOARD_WIDTH];	//Kepler 252B
Barrier _barrier;						//Barrier object
Cells _cells;							//Cell statistics
//--Global Variables--//

void setBoard() {
    int i, j, val;
    for (i = 0; i < BOARD_HEIGHT; i++) {
        for (j = 0; j < BOARD_WIDTH; j++) {
            val = rand() % 2;
			if (val) _cells.alive++;
			else _cells.died++;
			board[i][j] = val;
        }
    }
}

void printBoard() {
	//if (generations % 500 == 0) sleep(1);
    CLEAR_SCREEN();
	printf("Generations: %d || Alive: %d || Maintained: %d || Died: %d\n", STATISTICS);
	generations++;
	int i, j;
    for (i = 0; i < BOARD_HEIGHT; i++) {
        for (j = 0; j < BOARD_WIDTH; j++) {
            if (board[i][j]) printf(" . ");
            else printf("  ");
        }
        printf("\n");
    }
}

int xadd (int i, int a) {
	i += a;
	while (i < 0) i += BOARD_WIDTH;
	while (i >= BOARD_WIDTH) i -= BOARD_WIDTH;
	return i;
}

/* add to a height index, wrapping around */

int yadd (int i, int a) {
	i += a;
	while (i < 0) i += BOARD_HEIGHT;
	while (i >= BOARD_HEIGHT) i -= BOARD_HEIGHT;
	return i;
}

int adjacent_to (int i, int j) {
	int	k, l, count;
	count = 0;
	/* go around the cell */
	for (k = -1; k <= 1; k++) {
		for (l = -1; l <= 1; l++) {
		/* only count if at least one of k,l isn't zero */
			if (k || l) {
				if (board[xadd(i,k)][yadd(j,l)]) {
					count++;
				}
			}
		}
	}
	return count;
}

void* thrive(void* param) {
	do {
		int threadrow = (intptr_t) param;
		int	i, j, a;
		int newboard[BOARD_WIDTH];
		/* for each cell, apply the rules of Life */
		for (j = 0; j < BOARD_WIDTH; j++) {
			a = adjacent_to (threadrow, j);
			if (a == 2) {
				newboard[j] = board[threadrow][j];
				_cells.maintained++;
			}
			if (a == 3) {
				newboard[j] = 1;
				_cells.alive++;
			}
			if (a < 2 || a > 3) {
				newboard[j] = 0; 
				_cells.died++;
			}
		}
		barrierCloseRelease(&_barrier, BOARD_HEIGHT);
		/* copy the new board back into the old board */
		//CRITICAL SECTION START//
		for (j = 0; j < BOARD_WIDTH; j++) {
			board[threadrow][j] = newboard[j];
		}
		//CRITICAL SECTION END//
		if (threadrow == 0) printBoard();
		barrierCloseRelease(&_barrier, BOARD_HEIGHT);
	} while(1);
}

int main () {
	srand(time(NULL));
    pthread_t thread[BOARD_HEIGHT];
	_barrier = barrierInitialization();
	_cells = cellInitialization();
    setBoard();
    int i;
    for (i = 0; i < BOARD_HEIGHT; i++) {
        pthread_create(&thread[i], NULL, &thrive, (void*)(intptr_t)i);
	}
	do {;} while(1);
}
