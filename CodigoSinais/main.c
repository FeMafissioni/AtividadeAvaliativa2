#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define NUM_THREADS 5

volatile sig_atomic_t signalTemp = 1;

void handleSignal(int sigint) {
    signalTemp = 0;
}

void* counting_thread(void* thread_id) {
    long tid = (long)thread_id;
    int counter = 0;

    while (signalTemp) {
        printf("Thread %ld: count = %d\n", tid, counter++);
        sleep(1);
    }

    printf("Thread %ld finished.\n", tid);
    return NULL;
}

int main() {
    signal(SIGINT, handleSignal);

    pthread_t threads[NUM_THREADS];

    for (long i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, counting_thread, (void*)i) != 0) {
            fprintf(stderr, "Erro: não foi possível criar a thread %ld\n", i);
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Programa finalizado.\n");
    return 0;
}
