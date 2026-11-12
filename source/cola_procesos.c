#include "cola_procesos.h"

#include "configuracion.h"

#include <stdio.h>
#include <stdlib.h>

PCB_t *create_processQueue(void)
{
    PCB_t *node = malloc(sizeof(PCB_t));
    node->prev = node;
    node->pid = 1;
    node->lifetime = 56;
    node->next = NULL;
    return node;
}

void print_queue(PCB_t *node)
{
    while (node != NULL)
    {
        printf("Pid: %d \n Lifetime: %lf \n", node->pid, node->lifetime);
        node = node->next;
    }
}

int enqueue(PCB_t **head, PCB_t *node)
{
    node->next = NULL;
    if (*head == NULL)
    {
        *head = node;
        node->prev = node;
        return 0;
    }
    PCB_t *aux = (*head)->prev;
    aux->next = node;
    node->prev = aux;
    (*head)->prev = node;

    return 0;
}

PCB_t *dequeue(PCB_t **head)
{
    if (head == NULL || *head == NULL)
        return NULL;
    PCB_t *node = *head;
    (*head) = node->next;
    if (*head != NULL)
        (*head)->prev = node->prev;

    node->prev = NULL;
    node->next = NULL;
    return node;
}

int addRandomNode(PCB_t **queue)
{
    PCB_t *node = malloc(sizeof(PCB_t));
    if (node == NULL)
        return 1;

    node->prev = NULL;
    node->pid = pid;
    node->lifetime = rand() % 1024;
    node->next = NULL;

    pid++;
    return enqueue(queue, node);
}

void test_queue(void)
{
    PCB_t *queue = create_processQueue();
    for (int i = 0; i < 5; i++)
    {
        addRandomNode(&queue);
    }
    print_queue(queue);

    printf("------------------------\n");

    PCB_t *node;
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

void free_processQueue(PCB_t *queue)
{
    while (queue != NULL)
    {
        PCB_t *aux = queue;
        queue = queue->next;
        free(aux);
    }
}
