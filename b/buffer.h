#ifndef BUFFER_H
#define BUFFER_H

#include "mutex.h"
#include "semaforos.h"
#include "var_cond.h"
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 10 // Define o tamanho do buffer

// Estrutura do buffer compartilhado
typedef struct {
    int buffer[BUFFER_SIZE];  // Vetor para armazenar os itens
    int count;                 // Contador de itens no buffer
    int in;                    // Índice para inserção de novos itens
    int out;                   // Índice para remoção de itens
    semaforo_t empty_slots;    // Semáforo para controlar espaços vazios
    semaforo_t full_slots;     // Semáforo para controlar espaços ocupados
    mutex_t buffer_mutex;      // Mutex para proteger o buffer compartilhado
} shared_buffer_t;

// Função para inicializar o buffer compartilhado
void init_shared_buffer(shared_buffer_t *buffer) {
    buffer->count = 0;                  // Inicializa o contador de itens
    buffer->in = 0;                     // Inicializa o índice de inserção
    buffer->out = 0;                    // Inicializa o índice de remoção
    semaforo_init(&buffer->empty_slots, BUFFER_SIZE);  // Inicializa o semáforo de espaços vazios
    semaforo_init(&buffer->full_slots, 0);  // Inicializa o semáforo de espaços ocupados
    mutex_init(&buffer->buffer_mutex);    // Inicializa o mutex do buffer
}

#endif // BUFFER_H