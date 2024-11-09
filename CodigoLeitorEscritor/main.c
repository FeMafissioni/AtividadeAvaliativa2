#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int recurso = 0;              // Recurso compartilhado
int leitores = 0;             // Número de leitores ativos
pthread_mutex_t mutex;        // Mutex para gerenciar contagem de leitores
pthread_mutex_t recurso_mutex; // Mutex para gerenciar acesso exclusivo ao recurso

// Função para threads leitoras
void* leitor(void* arg) {
    long id = (long)arg;

    while (1) {
        // Seção de entrada para leitura
        pthread_mutex_lock(&mutex);
        leitores++;
        if (leitores == 1) {
            // Primeiro leitor bloqueia o acesso ao recurso
            pthread_mutex_lock(&recurso_mutex);
        }
        pthread_mutex_unlock(&mutex);

        // Seção crítica (leitura)
        printf("Leitor %ld lendo valor: %d\n", id, recurso);
        sleep(1); // Simula o tempo de leitura

        // Seção de saída
        pthread_mutex_lock(&mutex);
        leitores--;
        if (leitores == 0) {
            // Último leitor libera o recurso para escritores
            pthread_mutex_unlock(&recurso_mutex);
        }
        pthread_mutex_unlock(&mutex);

        sleep(1); // Simula tempo entre leituras
    }
    return NULL;
}

// Função para threads escritoras
void* escritor(void* arg) {
    long id = (long)arg;

    while (1) {
        // Seção de entrada para escrita
        pthread_mutex_lock(&recurso_mutex);

        // Seção crítica (escrita)
        recurso++;
        printf("Escritor %ld escrevendo valor: %d\n", id, recurso);
        sleep(2); // Simula o tempo de escrita

        // Seção de saída
        pthread_mutex_unlock(&recurso_mutex);

        sleep(2); // Simula tempo entre escritas
    }
    return NULL;
}

int main() {
    pthread_t leitores_threads[3], escritores_threads[2];
    long i;

    // Inicializa os mutexes
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&recurso_mutex, NULL);

    // Cria threads leitoras
    for (i = 0; i < 3; i++) {
        if (pthread_create(&leitores_threads[i], NULL, leitor, (void*)i) != 0) {
            fprintf(stderr, "Erro ao criar a thread leitora %ld\n", i);
            exit(1);
        }
    }

    // Cria threads escritoras
    for (i = 0; i < 2; i++) {
        if (pthread_create(&escritores_threads[i], NULL, escritor, (void*)i) != 0) {
            fprintf(stderr, "Erro ao criar a thread escritora %ld\n", i);
            exit(1);
        }
    }

    // Espera todas as threads (no caso de um programa real, teria uma condição de parada)
    for (i = 0; i < 3; i++) {
        pthread_join(leitores_threads[i], NULL);
    }
    for (i = 0; i < 2; i++) {
        pthread_join(escritores_threads[i], NULL);
    }

    // Destroi os mutexes
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&recurso_mutex);

    return 0;
}
