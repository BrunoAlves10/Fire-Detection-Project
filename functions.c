#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "functions.h"

char forest[SIZE][SIZE];
pthread_mutex_t mutex[SIZE][SIZE];
pthread_cond_t cond[SIZE][SIZE];
pthread_t sensors[SIZE][SIZE];
pthread_t fire_thread, control_thread;
int fire_active = 1;
int logs[SIZE][SIZE] = {0};

void init_forest() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (rand() % 2 == 0) {
                forest[i][j] = SENSOR;
            } else {
                forest[i][j] = EMPTY;
            }
            pthread_mutex_init(&mutex[i][j], NULL);
            pthread_cond_init(&cond[i][j], NULL);
        }
    }
}

void print_forest() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%c ", forest[i][j]);
        }
        printf("\n");
    }
}


void* fire_generator(void* arg) {
    while (fire_active) {
        sleep(3);
        int x = rand() % SIZE;
        int y = rand() % SIZE;

        pthread_mutex_lock(&mutex[x][y]);
        if (forest[x][y] == SENSOR) {
            forest[x][y] = FIRE;
            printf("Incendio iniciado em [%d, %d]\n", x, y);
            pthread_cond_signal(&cond[x][y]);
        }
        pthread_mutex_unlock(&mutex[x][y]);

        print_forest();
        printf("\n");
    }
    return NULL;
}

void* sensor_node(void* arg) {
    Position* pos = (Position*)arg;
    int x = pos->x;
    int y = pos->y;

    while (fire_active) {
        pthread_mutex_lock(&mutex[x][y]);
        while (forest[x][y] != FIRE) {
            pthread_cond_wait(&cond[x][y], &mutex[x][y]);
        }
        if (forest[x][y] == FIRE && logs[x][y] == 0) {
            printf("Sensor [%d, %d] detectou fogo!\n", x, y);
            logs[x][y] = 1;

            if (x == 0 || x == SIZE-1 || y == 0 || y == SIZE-1) {
                pthread_cond_signal(&cond[SIZE-1][SIZE-1]);
            }
        }
        pthread_mutex_unlock(&mutex[x][y]);
    }
    return NULL;
}

void* control_center(void* arg) {
    while (fire_active) {
        sleep(1);
        pthread_mutex_lock(&mutex[SIZE-1][SIZE-1]);
        printf("Central: Monitorando bordas...\n");
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (forest[i][j] == FIRE) {
                    printf("Central: Combate ao fogo em [%d, %d]\n", i, j);
                    forest[i][j] = BURNED;
                }
            }
        }
        pthread_mutex_unlock(&mutex[SIZE-1][SIZE-1]);
    }
    return NULL;
}
