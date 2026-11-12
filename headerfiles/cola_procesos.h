#ifndef COLA_PROCESOS_H
#define COLA_PROCESOS_H

#include "estructuras.h"

PCB_t *create_processQueue(void);
void print_queue(PCB_t *node);
int enqueue(PCB_t **head, PCB_t *node);
PCB_t *dequeue(PCB_t **head);
int addRandomNode(PCB_t **queue);
void test_queue(void);
void free_processQueue(PCB_t *queue);

#endif
