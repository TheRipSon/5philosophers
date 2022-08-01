#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define COUNT 5
int check[COUNT]; //tab for eat count
static pthread_mutex_t forks[COUNT];

void *philosopher(void *arg) {
    srand(time(NULL));   //init random
    int i = *(int *) arg;  //convert parameter from void* to int and assign value to i
    free(arg); //clear memory allocated in main
    int z = 0;
    while (z < 10) {
        z++;
        if (i == COUNT - 1) { // easiest solution one philosopher have different pick order
            pthread_mutex_lock(&forks[i]); // lock left fork
            printf("philosopher %d pick fork %d\n", i, i);
            pthread_mutex_lock(&forks[(i + 1) % COUNT]);// lock right fork
            printf("philosopher %d pick fork %d\n", i, (i + 1) % COUNT);
            printf("philosopher %d eat\n", i, i);
            check[i]++; // increment check tab
            usleep(1 + rand() % 1000);
            pthread_mutex_unlock(&forks[i]); // release left fork
            printf("philosopher %d release fork %d\n", i, i);
            pthread_mutex_unlock(&forks[(i + 1) % COUNT]); // release right fork
            printf("philosopher %d release fork %d\n", i, (i + 1) % COUNT);
        } else {
            pthread_mutex_lock(&forks[(i + 1) % COUNT]); // lock right fork
            printf("philosopher %d pick fork %d\n", i, (i + 1) % COUNT);
            pthread_mutex_lock(&forks[i]); // lock left fork
            printf("philosopher %d pick fork %d\n", i, i);
            printf("philosopher %d eat\n", i, i);
            check[i]++; // increment check tab
            usleep(1 + rand() % 1000);
            pthread_mutex_unlock(&forks[(i + 1) % COUNT]); // release right fork
            printf("philosopher %d release fork %d\n", i, (i + 1) % COUNT);
            pthread_mutex_unlock(&forks[i]); // release left fork
            printf("philosopher %d release fork %d\n", i, i);
        }
        printf("philosopher %d think\n", i, i);
        usleep(1 + rand() % 1000);
    }
    return NULL;
}


int main(int argc, char *argv[]) {
    int i;
    for (i = 0; i < COUNT; i++) {
        check[i] = 0; //initialize check tab
        pthread_mutex_init(&forks[i], NULL); // init mutex
    }
    pthread_t *tab = (pthread_t *) malloc(sizeof(pthread_t) * COUNT); // reserve memory for threads tab
    for (i = 0; i < COUNT; i++) {
        int *a = (int *) malloc(sizeof(int)); //reserve memory for int (id of thread)
        *a = i; // assign value to memory
        if (pthread_create(&tab[i], NULL, &philosopher, a) != 0) { //add routine and pointer(id thread) to thread
            perror("Failed to create thread");
        }
    }
    for (i = 0; i < COUNT; i++) {
        if (pthread_join(tab[i], NULL) != 0) { // start threads
            perror("Failed to join thread");
        }
    }
    for (i = 0; i < COUNT; i++) {
        printf("philosopher %d: %d\n", i, check[i]);
        pthread_mutex_destroy(&forks[i]); // free memory
    }
    return 0;
}