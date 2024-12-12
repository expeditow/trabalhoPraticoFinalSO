#ifndef PRODUCER_H
#define PRODUCER_H

#include "buffer.h" // Inclui a definição do buffer compartilhado

// Função que produz um item no buffer
void produce(shared_buffer_t *buffer, int item) {
    semaforo_wait(&buffer->empty_slots);  // Aguarda até que haja espaço no buffer
    mutex_lock(&buffer->buffer_mutex);    // Adquire o mutex para acessar o buffer

    buffer->buffer[buffer->in] = item;   // Insere o item no buffer
    buffer->in = (buffer->in + 1) % BUFFER_SIZE; // Atualiza o índice de inserção
    buffer->count++;                      // Incrementa o contador de itens

    mutex_unlock(&buffer->buffer_mutex);  // Libera o mutex
    semaforo_signal(&buffer->full_slots); // Sinaliza que há um item no buffer
}

#endif // PRODUCER_H