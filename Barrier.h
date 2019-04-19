#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
    sem_t mutex, turnstile, turnstile2;
    int COUNT;
} Barrier;

Barrier barrierInitialization() {
    Barrier X;
    sem_init(&X.mutex, 0, 1);
	sem_init(&X.turnstile, 0, 0);
	sem_init(&X.turnstile2, 0, 0);
    X.COUNT = 0;
    return X;
}

void barrierClose(Barrier** barrier, int BOARD_HEIGHT) {
    sem_wait(&(*barrier)->mutex);
    (*barrier)->COUNT++;
	//printf("Count: %d\n", (*barrier)->COUNT);
    if ((*barrier)->COUNT == BOARD_HEIGHT) {
        for (int i = 0; i < BOARD_HEIGHT; i++) {
            sem_post(&(*barrier)->turnstile);
		}
    }
    sem_post(&(*barrier)->mutex);
    sem_wait(&(*barrier)->turnstile);
}

void barrierRelease(Barrier** barrier, int BOARD_HEIGHT) {
    sem_wait(&(*barrier)->mutex);
    (*barrier)->COUNT--;
	//printf("Count: %d\n", (*barrier)->COUNT);
    if ((*barrier)->COUNT == 0) {
        for (int i = 0; i < BOARD_HEIGHT; i++) {
            sem_post(&(*barrier)->turnstile2);
		}
    }
    sem_post(&(*barrier)->mutex);
    sem_wait(&(*barrier)->turnstile2);
}

void barrierCloseRelease(Barrier* _barrier, int BOARD_HEIGHT) {
	barrierClose(&_barrier, BOARD_HEIGHT);
	barrierRelease(&_barrier, BOARD_HEIGHT);
}