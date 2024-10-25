#include <stdio.h>
#include <pthread.h>
#include "functions.h"

int main() {
    srand(time(NULL));
    init_forest();

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (forest[i][j] == SENSOR) {
                Position* pos = malloc(sizeof(Position));
                pos->x = i;
                pos->y = j;
                pthread_create(&sensors[i][j], NULL, sensor_node, pos);
            }
        }
    }

    pthread_create(&fire_thread, NULL, fire_generator, NULL);
    pthread_create(&control_thread, NULL, control_center, NULL);

    pthread_join(fire_thread, NULL);
    pthread_join(control_thread, NULL);

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (forest[i][j] == SENSOR) {
                pthread_join(sensors[i][j], NULL);
            }
        }
    }

    return 0;
}
