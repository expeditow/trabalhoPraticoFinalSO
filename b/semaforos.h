#ifndef SEMAFOROS_H
#define SEMAFOROS_H

#include "mutex.h"     // Inclui o arquivo do mutex
#include "var_cond.h"  // Inclui o arquivo da variável de condição
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

// Definição da estrutura do semáforo
typedef struct {
    int valor;             // Valor do semáforo
    mutex_t mutex;         // Mutex para proteger o semáforo
    cond_var_t cond_var;   // Variável de condição associada ao semáforo
} semaforo_t;

// Função para inicializar o semáforo
void semaforo_init(semaforo_t *sem, int valor_inicial) {
    sem->valor = valor_inicial;  // Inicializa o valor do semáforo
    mutex_init(&sem->mutex);     // Inicializa o mutex do semáforo
    cond_var_init(&sem->cond_var); // Inicializa a variável de condição do semáforo
}

// Função para destruir o semáforo
void semaforo_destroy(semaforo_t *sem) {
    mutex_destroy(&sem->mutex);    // Destrói o mutex do semáforo
    cond_var_destroy(&sem->cond_var); // Destrói a variável de condição do semáforo
}

// Função para esperar no semáforo
void semaforo_wait(semaforo_t *sem) {
    mutex_lock(&sem->mutex); // Adquire o mutex para proteger o valor do semáforo
    while (sem->valor <= 0) { // Espera até que o semáforo tenha valor positivo
        cond_var_wait(&sem->cond_var, &sem->mutex); // Espera na variável de condição
    }
    sem->valor--; // Decrementa o valor do semáforo
    mutex_unlock(&sem->mutex); // Libera o mutex
}

// Função para sinalizar o semáforo
void semaforo_signal(semaforo_t *sem) {
    mutex_lock(&sem->mutex); // Adquire o mutex para proteger o valor do semáforo
    sem->valor++; // Incrementa o valor do semáforo
    cond_var_signal(&sem->cond_var); // Sinaliza uma thread esperando na variável de condição
    mutex_unlock(&sem->mutex); // Libera o mutex
}

#endif // SEMAFOROS_H