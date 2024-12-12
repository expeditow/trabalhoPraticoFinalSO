#ifndef COND_VARIAVEIS_H
#define COND_VARIAVEIS_H

#include "mutex.h" 
#include <stdlib.h>
#include <stdio.h>

// Definição da estrutura da variável de condição
typedef struct {
    int waiters;        // Contador de threads esperando
    int signal_count;   // Contador de sinais emitidos
    mutex_t mutex;      // Mutex para proteger a variável de condição
} cond_var_t;

// Função para inicializar a variável de condição
void cond_var_init(cond_var_t *cond) {
    cond->waiters = 0;      // Inicializa o contador de threads esperando
    cond->signal_count = 0; // Inicializa o contador de sinais emitidos
    mutex_init(&cond->mutex); // Inicializa o mutex da variável de condição
}

// Função para destruir a variável de condição
void cond_var_destroy(cond_var_t *cond) {
    mutex_destroy(&cond->mutex); // Destrói o mutex da variável de condição
}

// Função para esperar na variável de condição
void cond_var_wait(cond_var_t *cond, mutex_t *external_mutex) {
    mutex_lock(&cond->mutex);    // Adquire o mutex da variável de condição
    cond->waiters++;             // Incrementa o contador de threads esperando
    mutex_unlock(&cond->mutex);  // Libera o mutex da variável de condição

    mutex_unlock(external_mutex); // Libera o mutex externo para outras threads

    while (1) {  // Laço de espera até ser sinalizado
        mutex_lock(&cond->mutex); 
        if (cond->signal_count > 0) {  // Verifica se houve sinalização
            cond->signal_count--;  // Decrementa o contador de sinais
            cond->waiters--;       // Decrementa o contador de threads esperando
            mutex_unlock(&cond->mutex); // Libera o mutex da variável de condição
            break;
        }
        mutex_unlock(&cond->mutex);  // Libera o mutex da variável de condição
    }

    mutex_lock(external_mutex);  // Adquire o mutex externo após acordar
}

// Função para sinalizar uma thread esperando
void cond_var_signal(cond_var_t *cond) {
    mutex_lock(&cond->mutex);    // Adquire o mutex da variável de condição
    if (cond->waiters > 0) {     // Verifica se há threads esperando
        cond->signal_count++;    // Incrementa o contador de sinais
    }
    mutex_unlock(&cond->mutex);  // Libera o mutex da variável de condição
}

// Função para sinalizar todas as threads esperando
void cond_var_broadcast(cond_var_t *cond) {
    mutex_lock(&cond->mutex);    // Adquire o mutex da variável de condição
    cond->signal_count += cond->waiters;  // Incrementa o contador de sinais para todas as threads
    mutex_unlock(&cond->mutex);  // Libera o mutex da variável de condição
}

#endif // COND_VARIABLES_H