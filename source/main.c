#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "estructuras.h"

void print_queue(PCB *node);
int addRandomNode(PCB **queue);
void test_queue();
void printMachineParameters();

int clockFreq = 0;
int ticks_per_clock = 0;
int duration = 0;
Machine machine;

int main(int argc, char *argv[])
{
    // srand(time(NULL));
    //pthread_t machine_creation_thread;

    setParameters( argv, argc);

    printMachineParameters();

    // if (pthread_create(&machine_creation_thread, NULL, (void *)spawnMachine, NULL) != 0)
    // {
    //     fprintf(stderr, "Error creating machine creation thread\n");
    //     return 1;
    // }

    PCB *processQueue = create_processQueue();

    // test_queue();
    // code

    free_processQueue(processQueue);

   // pthread_join(machine_creation_thread, NULL);
}

PCB *create_processQueue()
{
    PCB *node = malloc(sizeof(PCB));
    node->prev = node;
    node->pid = 1;
    node->lifetime = 56;
    node->next = NULL;
    return node;
}

void print_queue(PCB *node)
{
    while (node != NULL)
    {
        printf("Pid: %d \n Lifetime: %lf \n", node->pid, node->lifetime);
        node = node->next;
    }
}

// Encola el nodo al final de la cola
int enqueue(PCB **head, PCB *node)
{
    node->next = NULL;
    if (*head == NULL)
    {
        *head = node;
        node->prev = node;
        return 0;
    }
    PCB *aux = (*head)->prev;
    aux->next = node;
    node->prev = aux;
    (*head)->prev = node;

    return 0;
}

// Devuelve el primer nodo y cambia la cabeza de la cola
PCB *dequeue(PCB **head)
{
    if (head == NULL || *head == NULL)
        return NULL;
    PCB *node = *head;
    (*head) = node->next;
    if (*head != NULL)
        (*head)->prev = node->prev;

    node->prev = NULL;
    node->next = NULL;
    return node;
}

int addRandomNode(PCB **queue)
{
    PCB *node = malloc(sizeof(PCB));
    if (node == NULL)
        return 1; // malloc failed

    node->prev = NULL;
    node->pid = rand() % 255;
    node->lifetime = rand() % 1024;
    node->next = NULL;

    return enqueue(queue, node);
}

void test_queue()
{

    PCB *queue = create_processQueue();
    for (int i = 0; i < 5; i++)
    {
        addRandomNode(&queue);
    }
    print_queue(queue);

    printf("------------------------\n");

    PCB *node;
    node = dequeue(&queue);
    print_queue(queue);
    printf("------------------------\n");
    enqueue(&queue, node);
    print_queue(queue);
    printf("------------------------\n");

    while ((node = dequeue(&queue)) != NULL)
        free(node);
    print_queue(queue);
    printf("------------------------\n");

    addRandomNode(&queue);
    print_queue(queue);

    printf("------------------------\n");

    free_processQueue(queue);
}

void free_processQueue(PCB *queue)
{
    while (queue != NULL)
    {
        PCB *aux = queue;
        queue = queue->next;
        free(aux);
    }
}

int clock_function(mutex_block *clock_mutex)
{
    int clock_counter = 0;
    double clock_interval = 1.0 / clockFreq; // Intervalo de tiempo entre ticks del reloj en segundos

    while (1)
    {
        if (clock_counter >= clockFreq * duration)
            break;
        pthread_mutex_lock(clock_mutex->mutex);
        sleep(clock_interval);
        clock_counter++;
        pthread_mutex_unlock(clock_mutex->mutex);
    }
    return 0;
}

int spawnClock(pthread_t *clock_thread, mutex_block *clock_mutex)
{
    return pthread_create(clock_thread, NULL, (void *)clock_function, clock_mutex);
}

int spawnTimer(pthread_t *timer_thread, thread_args *args)
{
    return pthread_create(timer_thread, NULL, (void *)timer_function, args);
}

int timer_function(thread_args *args)
{
    int clock = 0;
    while (1)
    {   
        pthread_mutex_lock(args->timer_mutex->mutex);
        clock++;
        if (args->pgen_mutex->flag == 0)
        {
            pthread_mutex_lock(args->pgen_mutex->mutex);
            args->pgen_mutex->flag = 1;
        }
        else if (args->scheduler_mutex->flag == 0)
        {
            pthread_mutex_lock(args->scheduler_mutex->mutex);
            args->scheduler_mutex->flag = 1;
        }
        else if (args->dispatcher_mutex->flag == 0)
        {
            pthread_mutex_lock(args->dispatcher_mutex->mutex);
            args->dispatcher_mutex->flag = 1;
        }

        if (clock % ticks_per_clock == 0)
        {
            if (args->pgen_mutex->flag == 1)
            {
                pthread_mutex_unlock(args->pgen_mutex->mutex);
                args->pgen_mutex->flag = 0;
            }
            else if (args->scheduler_mutex->flag == 1)
            {
                pthread_mutex_unlock(args->scheduler_mutex->mutex);
                args->scheduler_mutex->flag = 0;
            }
            else if (args->dispatcher_mutex->flag == 1)
            {
                pthread_mutex_unlock(args->dispatcher_mutex->mutex);
                args->dispatcher_mutex->flag = 0;
            }
        }
        pthread_mutex_unlock(args->timer_mutex->mutex);
    }
    return 0;
}

int spawnMachine()
{
    pthread_t clock_thread, timer_thread, pgen_thread, scheduler_thread, dispatcher_thread;

    pthread_mutex_t timer_clock_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t pgen_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t scheduler_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t dispatcher_mutex = PTHREAD_MUTEX_INITIALIZER;

    mutex_block timer_mutex = {&timer_clock_mutex, 0};
    mutex_block pgen_mutex_block = {&pgen_mutex, 0};
    mutex_block scheduler_mutex_block = {&scheduler_mutex, 0};
    mutex_block dispatcher_mutex_block = {&dispatcher_mutex, 0};

    thread_args args = {&timer_mutex, &pgen_mutex_block, &scheduler_mutex_block, &dispatcher_mutex_block};

    printf("Spawning machine with %d CPUs, %d cores, and %d threads.\n", machine.ncpu, machine.ncore, machine.nthreads);

    if (spawnClock(&clock_thread, &timer_mutex) != 0)
    {
        fprintf(stderr, "Error creating clock thread\n");
        return 1;
    }

    if (spawnTimer(&timer_thread, &args) != 0)
    {
        fprintf(stderr, "Error creating timer thread\n");
        return 1;
    }


    pthread_join(clock_thread, NULL);
    pthread_join(timer_thread, NULL);
    // pthread_join(pgen_thread, NULL);
    // pthread_join(scheduler_thread, NULL);
    // pthread_join(dispatcher_thread, NULL);


    return 0;
}

int setParameters( char *argv[], int argc)
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
            printf("Falta un parametro obligatorio\n");
            return 1;
        }
    }
}
void printMachineParameters()
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
