#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

/* Args structure to send data to threads */
typedef struct thread_args {
    unsigned long int destination_id;
} thread_args;

/* Semaphore to synchronize test tasks */
sem_t test_start;