#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

#include <pthread.h>

typedef struct Machine
{
    int ncpu;
    int ncore;
    int nthreads;
    HWThread *hwthreads;
    int totalThreads;
    
} Machine;

typedef struct HWThread
{
    int occupied;
    PCB_t *assignedProcess;

} HWThread;

typedef struct PCB_t
{
    struct PCB_t *prev;
    int pid;
    double lifetime;
    struct PCB_t *next;
    enum ProcessState state;
    double remainingTime;
    double arrivalTime;
    int priority;
    int quantumLeft;
    pthread_t *assignedThread;
} PCB_t;

typedef struct mutex_block_t
{
    pthread_mutex_t *mutex;
    int flag;
} mutex_block_t;

typedef struct thread_args_t
{
    mutex_block_t *timer_mutex;
    mutex_block_t *pgen_mutex;
    mutex_block_t *scheduler_mutex;
    mutex_block_t *dispatcher_mutex;
} thread_args_t;

typedef struct pgen_args_t
{
    mutex_block_t *pgen_mutex;
    PCB_t **processQueue;
} pgen_args_t;

typedef struct scheduler_args_t
{
    mutex_block_t *scheduler_mutex;
    PCB_t **processQueue;
} scheduler_args_t;

typedef struct dispatcher_args_t
{
    mutex_block_t *dispatcher_mutex;
    PCB_t **processQueue;
} dispatcher_args_t;


enum ProcessState
{
    READY,
    RUNNING,
    FiNISHED
};
#endif
