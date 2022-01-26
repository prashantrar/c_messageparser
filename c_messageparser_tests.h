#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

typedef struct thread_args {
    int destination_id;
} thread_args;
sem_t test_start;