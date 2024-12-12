#ifndef CONSUMER_H
#define CONSUMER_H

#include "buffer.h" // Inclui a definição do buffer compartilhado

// Função que consome um item do buffer
int consume(shared_buffer_t *buffer) {
    semaforo_wait(&buffer->full_slots);  // Aguarda até que haja um item no buffer
    mutex_lock(&buffer->buffer_mutex);   // Adquire o mutex para acessar o buffer

    int item = buffer->buffer[buffer->out]; // Remove o item do buffer
    buffer->out = (buffer->out + 1) % BUFFER_SIZE; // Atualiza o índice de remoção
    buffer->count--; // Decrementa o contador de itens

    mutex_unlock(&buffer->buffer_mutex);  // Libera o mutex
    semaforo_signal(&buffer->empty_slots); // Sinaliza que há um espaço vazio no buffer

    return item; // Retorna o item consumido
}

#endif // CONSUMER_H