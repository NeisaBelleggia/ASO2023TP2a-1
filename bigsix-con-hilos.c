#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>

#define NUMBER_OF_THREADS 2
#define CANTIDAD_INICIAL_HAMBURGUESAS 20

int cantidad_restante_hamburguesas = CANTIDAD_INICIAL_HAMBURGUESAS;
atomic_int turn = ATOMIC_VAR_INIT(0);

void *comer_hamburguesa(void *tid) {
    while (1) {
        while (atomic_load(&turn) != (long)tid) {
            // Espera activa hasta que sea el turno de este hilo
        }

        if (cantidad_restante_hamburguesas > 0) {
            printf("Hola! Soy el hilo (comensal) %ld, me voy a comer una hamburguesa! Quedan %d\n", (long)tid, cantidad_restante_hamburguesas);
            cantidad_restante_hamburguesas--;
        } else {
            printf("SE TERMINARON LAS HAMBURGUESAS %ld :(\n", (long)tid);
            pthread_exit(NULL);
        }

        atomic_store(&turn, ((long)tid + 1) % NUMBER_OF_THREADS); // Cambiar el turno al siguiente hilo
    }
}

int main(int argc, char *argv[]) {
    pthread_t threads[NUMBER_OF_THREADS];
    int status, i, ret;

    for (i = 0; i < NUMBER_OF_THREADS; i++) {
        printf("Hola!, soy el hilo principal. Estoy creando el hilo %d\n", i);
        status = pthread_create(&threads[i], NULL, comer_hamburguesa, (void *)(long)i);
        if (status != 0) {
            printf("Algo salió mal, al crear el hilo recibi el código de error %d\n", status);
            exit(-1);
        }
    }

    for (i = 0; i < NUMBER_OF_THREADS; i++) {
        void *retval;
        ret = pthread_join(threads[i], &retval);
    }

    pthread_exit(NULL);
}