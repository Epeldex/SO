#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "estructuras.h"

void print_queue(PCB *node);
int addRandomNode(PCB **queue);
void test_queue();

int main(int argc, char *argv[])
{
    srand(time(NULL));

    PCB *processQueue = create_processQueue();

    test_queue();
    // code

    free_processQueue(processQueue);

    int clockFreq =0;
    int timerfreq =0;
    int duration =0;
    struct Machine machine;
    int found[6] = {-1, -1, -1, -1, -1, -1};
    
    for (int i = 1; i<argc; i+=2){
         
        if (i + 1 >= argc) {
            printf("Falta el valor del parametro %s\n", argv[i]);
            return 1;
            }

    if(strcmp(argv[i], "-cpus") == 0 && found[0]==-1){

        machine.ncpu=atoi(argv[i+1]);
        found[0]=machine.ncpu;

    } else if (strcmp(argv[i], "-cores") == 0 && found[1]==-1) {

        machine.ncore=atoi(argv[i+1]);
        found[1]=machine.ncore;
    }
    else if (strcmp(argv[i], "-hwthreads")== 0 && found[2]==-1)  {

        machine.nthreads=atoi(argv[i+1]);
        found[2]=machine.nthreads;

    }else if(strcmp(argv[i], "-clock-freq") == 0 && found[3]==-1){
        clockFreq=atoi(argv[i+1]);
        found[3]=clockFreq;
    }
    else if(strcmp(argv[i], "-timer-freq") == 0 && found[4]==-1){
        timerfreq=atoi(argv[i+1]);
        found[4]=timerfreq;

    }
    else if(strcmp(argv[i], "-duration") == 0 && found[5]==-1){
        duration=atoi(argv[i+1]);
        found[5]=duration;
    }
    
    else {
        printf("El parametro %s no existe \n", argv[i]);
        return 1;
    }
    }
    for (int i = 0; i < 6; i++) {
        if (found[i] == -1) {
            printf("Falta un parametro obligatorio\n");
            return 1;
        }
    }
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
