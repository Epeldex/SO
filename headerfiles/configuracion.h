#ifndef CONFIGURACION_H
#define CONFIGURACION_H

#include "estructuras.h"

extern int pid;
extern int clockFreq;
extern int ticks_per_clock;
extern int duration;
extern Machine machine;

int setParameters(char *argv[], int argc);
void printMachineParameters(void);

#endif
