#ifndef MUTEX_H
#define MUTEX_H

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

// Definição da estrutura do mutex
typedef struct {
    volatile int locked; // Variável que indica se o mutex está bloqueado (1) ou desbloqueado (0)
} mutex_t;

// Função para inicializar o mutex
void mutex_init(mutex_t *mutex) {
    mutex->locked = 0; // Inicializa o mutex como desbloqueado
}

// Função para tentar adquirir o mutex com tempo limite
int mutex_lock_timeout(mutex_t *mutex, int timeout_sec) {
    struct timespec ts_start, ts_now;  // Variáveis para armazenar o tempo inicial e o tempo atual
    clock_gettime(CLOCK_REALTIME, &ts_start); // Obtém o tempo atual no início

    while (1) { // Laço de espera ativa
        if (__sync_bool_compare_and_swap(&mutex->locked, 0, 1)) { // Tenta adquirir o mutex de forma atômica
            return 0; 
        }

        clock_gettime(CLOCK_REALTIME, &ts_now); // Atualiza o tempo atual
        if ((ts_now.tv_sec - ts_start.tv_sec) >= timeout_sec) { // Verifica se o tempo limite foi atingido
            return -1; // Retorna erro caso não tenha conseguido adquirir o mutex
        }

        struct timespec ts_sleep = {0, 1000000}; // Aguarda 1 milissegundo antes de tentar novamente
        nanosleep(&ts_sleep, NULL);
    }
}

// Função para adquirir o mutex
void mutex_lock(mutex_t *mutex) {
    while (!__sync_bool_compare_and_swap(&mutex->locked, 0, 1)) { // Aguarda até adquirir o mutex
        struct timespec ts_sleep = {0, 1000000}; // Aguarda 1 milissegundo antes de tentar novamente
        nanosleep(&ts_sleep, NULL);
    }
}

// Função para liberar o mutex
void mutex_unlock(mutex_t *mutex) {
    __sync_bool_compare_and_swap(&mutex->locked, 1, 0); // Libera o mutex
}

// Função para destruir o mutex (resetar seu valor)
void mutex_destroy(mutex_t *mutex) {
    mutex->locked = 0; // Reseta o estado do mutex para desbloqueado
}

#endif // MUTEX_H