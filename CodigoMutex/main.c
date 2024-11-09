#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int contador = 0;
pthread_mutex_t lock1;
pthread_mutex_t lock2;

void* incrementar(void* arg) {
    pthread_mutex_lock(&lock1);
    sleep(1);

    pthread_mutex_lock(&lock2);

    contador++;
    printf("Thread 1: Contador = %d\n", contador);

    pthread_mutex_unlock(&lock2);
    pthread_mutex_unlock(&lock1);

    return NULL;
}

void* decrementar(void* arg) {
    pthread_mutex_lock(&lock2);
    sleep(1);
    pthread_mutex_lock(&lock1);
    contador--;
    printf("Thread 2: Contador = %d\n", contador);

    pthread_mutex_unlock(&lock1);
    pthread_mutex_unlock(&lock2);
    return NULL;
}

int main() {
    pthread_t t1, t2;

    pthread_mutex_init(&lock1, NULL);
    pthread_mutex_init(&lock2, NULL);

    pthread_create(&t1, NULL, incrementar, NULL);
    pthread_create(&t2, NULL, decrementar, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&lock1);
    pthread_mutex_destroy(&lock2);

    return 0;
}
