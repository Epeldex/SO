
typedef struct PCB {
    struct PCB *prev;
    int pid;
    double lifetime;
    struct PCB *next;
};

typedef struct Machine {
    int ncpu;
    int ncore;
    int nthreads;
};
