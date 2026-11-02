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

    return 0;
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
