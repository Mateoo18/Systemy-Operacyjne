
#ifndef SEMAFORY_H
#define SEMAFORY_H

sem_t *createsem (const char *name);
sem_t *opensem (const char *name);
void closesem (sem_t *sem_t);
void valsem (sem_t *sem, int *sval);
void incsem (sem_t *sem);
void levsem (sem_t *sem);
void erasem (const char *name);

#endif