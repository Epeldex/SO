typedef struct Machine
{
    int ncpu;
    int ncore;
    int nthreads;
} Machine;

typedef struct PCB_t
{
    struct PCB_t *prev;
    int pid;
    double lifetime;
    struct PCB_t *next;
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

PCB_t *create_processQueue();
int enqueue(PCB_t **head, PCB_t *node);
PCB_t *dequeue(PCB_t **head);
void free_processQueue(PCB_t *queue);
int setParameters(char *argv[], int argc);
int spawnMachine(PCB_t **processQueue);
int spawnClock(pthread_t *clock_thread, mutex_block_t *timer_mutex);
int clock_function(mutex_block_t *timer_mutex);
int spawnTimer(pthread_t *timer_thread, thread_args_t *args);
int timer_function(thread_args_t *args);
int spawnProcessGenerator(pthread_t *pgen_thread, pgen_args_t *args);
int process_generator_function(pgen_args_t *args);
