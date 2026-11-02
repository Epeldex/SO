#include <stdio.h>

#include "estructuras.h"

int main(int argc, char *argv[])
{
    struct PCB processQueue;

    processQueue.prev = NULL;
    processQueue.pid = 1;
    processQueue.lifetime = 56;
    processQueue.next = NULL;
}

void print_node(struct PCB *node)
{
    printf("Pid: %d \nLifetime: %lf", node->pid, node->lifetime);
}

struct PCB *push(struct PCB *head, struct PBC *node)
{
    struct PCB *aux = head;
    while (aux != NULL)
        aux = aux->next;
    aux->next = node;
    return head;
}

struct PCB *pop(struct PCB *node)
{
    struct PCB* new_head = node->next;
    new_head->prev = NULL;
    node->next = NULL;
    return new_head;
}
