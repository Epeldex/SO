#ifndef RELOJ_H
#define RELOJ_H

#include "estructuras.h"

int clock_function(mutex_block_t *clock_mutex);
int spawnClock(pthread_t *clock_thread, mutex_block_t *clock_mutex);

#endif
