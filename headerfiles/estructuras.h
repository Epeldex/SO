typedef struct Machine {
    int ncpu;
    int ncore;
    int nthreads;
};

typedef struct PCB {
    struct PCB *prev;
    int pid;
    double lifetime;
    struct PCB *next;
} PCB;

PCB *create_processQueue();
int enqueue(PCB **head, PCB *node);
PCB *dequeue(PCB **head);
void free_processQueue(PCB *queue);



