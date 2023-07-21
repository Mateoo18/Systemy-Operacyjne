#ifndef WATKI_H
#define WATKI_H

#include <pthread.h>

pthread_t create_thr (void *funkcja(void *), void *argument);
void join_thr (pthread_t watek);
void create_mutex (pthread_mutex_t *mutex);
void close_mutex (pthread_mutex_t *mutex);
void open_mutex (pthread_mutex_t *mutex);
void des_mutex (pthread_mutex_t *mutex);

#endif