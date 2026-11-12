#ifndef GENERADOR_PROCESOS_H
#define GENERADOR_PROCESOS_H

#include "estructuras.h"

int process_generator_function(pgen_args_t *args);
int spawnProcessGenerator(pthread_t *pgen_thread, pgen_args_t *pgen_args);

#endif
