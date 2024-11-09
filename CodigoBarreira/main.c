#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 5
#define NUM_FASES 3

// Declara a barreira
pthread_barrier_t barreira;

void* trabalho(void* thread_id) {
    long tid = (long)thread_id;

    for (int fase = 1; fase <= NUM_FASES; fase++) {
        printf("Thread %ld esta executando a fase %d\n", tid, fase);

        // Simula trabalho com sleep
        sleep(1);

        // Aguarda na barreira até que todas as threads alcancem este ponto
        printf("Thread %ld aguardando na barreira na fase %d\n", tid, fase);
        pthread_barrier_wait(&barreira);

        // Todas as threads prosseguem juntas para a próxima fase após a barreira
        printf("Thread %ld passou pela barreira na fase %d\n", tid, fase);
    }

    printf("Thread %ld finalizou todas as fases.\n", tid);
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];

    // Inicializa a barreira para sincronizar NUM_THREADS threads
    pthread_barrier_init(&barreira, NULL, NUM_THREADS);

    // Cria as threads
    for (long i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, trabalho, (void*)i) != 0) {
            fprintf(stderr, "Erro: não foi possível criar a thread %ld\n", i);
            exit(EXIT_FAILURE);
        }
    }

    // Aguarda todas as threads terminarem
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroi a barreira
    pthread_barrier_destroy(&barreira);

    printf("Programa finalizado.\n");
    return 0;
}
