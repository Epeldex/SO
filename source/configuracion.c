#include "configuracion.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int pid = 0;
int clockFreq = 0;
int ticks_per_clock = 0;
int duration = 0;
Machine machine;

int setParameters(char *argv[], int argc)
{
    int found[6] = {-1, -1, -1, -1, -1, -1};

    for (int i = 1; i < argc; i += 2)
    {
        if (i + 1 >= argc)
        {
            printf("Falta el valor del parametro %s\n", argv[i]);
            return 1;
        }

        if (strcmp(argv[i], "-cpus") == 0 && found[0] == -1)
        {
            machine.ncpu = atoi(argv[i + 1]);
            found[0] = machine.ncpu;
        }
        else if (strcmp(argv[i], "-cores") == 0 && found[1] == -1)
        {
            machine.ncore = atoi(argv[i + 1]);
            found[1] = machine.ncore;
        }
        else if (strcmp(argv[i], "-hwthreads") == 0 && found[2] == -1)
        {
            machine.nthreads = atoi(argv[i + 1]);
            found[2] = machine.nthreads;
        }
        else if (strcmp(argv[i], "-clock-freq") == 0 && found[3] == -1)
        {
            clockFreq = atoi(argv[i + 1]);
            found[3] = clockFreq;
        }
        else if (strcmp(argv[i], "-timer-freq") == 0 && found[4] == -1)
        {
            ticks_per_clock = atoi(argv[i + 1]);
            found[4] = ticks_per_clock;
        }
        else if (strcmp(argv[i], "-duration") == 0 && found[5] == -1)
        {
            duration = atoi(argv[i + 1]);
            found[5] = duration;
        }
        else
        {
            printf("El parametro %s no existe \n", argv[i]);
            return 1;
        }
    }

    for (int i = 0; i < 6; i++)
    {
        if (found[i] == -1)
        {
            switch (i)
            {
            case 0:
                machine.ncpu = 2;
                break;
            case 1:
                machine.ncore = 4;
                break;
            case 2:
                machine.nthreads = 2;
                break;
            case 3:
                clockFreq = 1;
                break;
            case 4:
                ticks_per_clock = 1;
                break;
            case 5:
                duration = 100;
                break;
            }
        }
    }
    return -1;
}

void printMachineParameters(void)
{
    printf("\n========== MACHINE PARAMETERS ==========\n");
    printf("CPUs:              %d\n", machine.ncpu);
    printf("Cores:             %d\n", machine.ncore);
    printf("Hardware threads:  %d\n", machine.nthreads);
    printf("----------------------------------------\n");
    printf("Clock frequency:   %d\n", clockFreq);
    printf("Timer frequency:   %d\n", ticks_per_clock);
    printf("Duration:          %d\n", duration);
    printf("========================================\n\n");
}
