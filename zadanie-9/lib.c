  #include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

pthread_t create_thr (void *funkcja(void *), void *argument) {
    pthread_t id_watku;
    int status = pthread_create (&id_watku, NULL, funkcja, argument);
   if(status!=0){
       printf("Blad watku");
       exit(EXIT_FAILURE);
   }
    return id_watku;
}

void join_thr (pthread_t watek) {
    if (pthread_join (watek, NULL) != 0) {
        printf("Błąd łączenia wątku!\n");
        exit(EXIT_FAILURE);
    }
}

void create_mutex (pthread_mutex_t *mutex) {
    if (pthread_mutex_init(mutex, NULL) != 0) {
        printf("Błąd przy inicjalizacji mutexu!\n");
        exit(EXIT_FAILURE);
    }
}

void close_mutex (pthread_mutex_t *mutex) {
    if (pthread_mutex_lock(mutex) != 0) {
        printf("Błąd zamykania mutexu!\n");
        exit(EXIT_FAILURE);
    }
}

void open_mutex (pthread_mutex_t *mutex) {
    if (pthread_mutex_unlock(mutex) != 0) {
        printf("Błąd otwierania mutexu!\n");
        exit(EXIT_FAILURE);
    }
}

void des_mutex (pthread_mutex_t *mutex) {
    if (pthread_mutex_destroy (mutex) != 0) {
        printf ("Błąd usuwania mutexu!\n");
        exit (EXIT_FAILURE);
    }
}