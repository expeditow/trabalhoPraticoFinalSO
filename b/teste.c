#include "produtor.h"
#include "consumidor.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 2
#define ITEMS_TO_PRODUCE 10

shared_buffer_t buffer;

void* producer_thread(void* arg) {
	int id = *((int*)arg);  
	for (int i = 0; i < ITEMS_TO_PRODUCE; i++) {
    	int item = id * 100 + i; 
    	printf("Produtor %d produzindo item %d\n", id, item);
    	produce(&buffer, item);
	}
	return NULL;
}

void* consumer_thread(void* arg) {
	int id = *((int*)arg);  
	for (int i = 0; i < ITEMS_TO_PRODUCE; i++) {
    	int item = consume(&buffer);
    	printf("Consumidor %d consumiu item %d\n", id, item);
	}
	return NULL;
}

int main() {
	pthread_t producers[NUM_PRODUCERS];
	pthread_t consumers[NUM_CONSUMERS];
	int producer_ids[NUM_PRODUCERS];
	int consumer_ids[NUM_CONSUMERS];

	init_shared_buffer(&buffer);

	for (int i = 0; i < NUM_PRODUCERS; i++) {
    	producer_ids[i] = i + 1;
    	if (pthread_create(&producers[i], NULL, producer_thread, &producer_ids[i]) != 0) {
        	perror("Falha ao criar thread do produtor");
        	exit(EXIT_FAILURE);
    	}
	}

	for (int i = 0; i < NUM_CONSUMERS; i++) {
    	consumer_ids[i] = i + 1;
    	if (pthread_create(&consumers[i], NULL, consumer_thread, &consumer_ids[i]) != 0) {
        	perror("Falha ao criar thread do consumidor");
        	exit(EXIT_FAILURE);
    	}
	}

	for (int i = 0; i < NUM_PRODUCERS; i++) {
    	pthread_join(producers[i], NULL);
	}

	for (int i = 0; i < NUM_CONSUMERS; i++) {
    	pthread_join(consumers[i], NULL);
	}

	printf("Teste concluído com sucesso.\n");

	return 0;
}
