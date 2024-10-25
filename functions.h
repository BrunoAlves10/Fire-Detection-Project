#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define SIZE 30
#define FIRE '@'
#define EMPTY '-'
#define SENSOR 'T'
#define BURNED '/'

typedef struct {
    int x, y;
} Position;

extern char forest[SIZE][SIZE];
extern pthread_mutex_t mutex[SIZE][SIZE];
extern pthread_cond_t cond[SIZE][SIZE];
extern pthread_t sensors[SIZE][SIZE];
extern pthread_t fire_thread, control_thread;
extern int fire_active;

void init_forest();
void print_forest();
void* sensor_node(void* arg);
void* fire_generator(void* arg);
void* control_center(void* arg);

#endif
