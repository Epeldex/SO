typedef struct Machine
{
    int ncpu;
    int ncore;
    int nthreads;
} Machine;

typedef struct PCB
{
    struct PCB *prev;
    int pid;
    double lifetime;
    struct PCB *next;
} PCB;

typedef struct mutex_block
{
    pthread_mutex_t *mutex;
    int flag;
} mutex_block;

typedef struct thread_args
{
    mutex_block *timer_mutex;
    mutex_block *pgen_mutex;
    mutex_block *scheduler_mutex;
    mutex_block *dispatcher_mutex;
} thread_args;

PCB *create_processQueue();
int enqueue(PCB **head, PCB *node);
PCB *dequeue(PCB **head);
void free_processQueue(PCB *queue);
int setParameters(char *argv[], int argc);
int spawnMachine();
int spawnClock(pthread_t *clock_thread, mutex_block *timer_mutex);
int clock_function(mutex_block *timer_mutex);
int spawnTimer(pthread_t *timer_thread, thread_args *args);
int timer_function(thread_args *args);
int spawnProcessGenerator(pthread_t *pgen_thread, thread_args *args);
int process_generator_function(mutex_block *pgen_mutex);
