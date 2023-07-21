#include <fcntl.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
void err(){
    perror ("blad semafora!");
    exit (EXIT_FAILURE);
}

sem_t *createsem (char *name) {
    sem_t *sem;
    if ((sem = sem_open(name, O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED) {
        err();
    } else {
        return sem;
    }
}

sem_t *opensem (char *name) {
    sem_t *sem;
    if ((sem = sem_open(name, 0)) == SEM_FAILED) {
        err();
    } else {
        return sem;
    }
}
void closesem (sem_t *sem_t) {
    if (sem_close (sem_t) == -1) {
       err();
    }
}
void valsem (sem_t *sem, int *sval) {
    if (sem_getvalue(sem, sval) == -1) {
        err();
    }
}
void incsem (sem_t *sem) {
    if (sem_post(sem) == -1) {
       err();
    }
}
void levsem(sem_t *sem) {
    if (sem_wait(sem) == -1) {
        err();
    }
}
void erasem ( char *name) {
    if (sem_unlink(name) == -1) {
        err();
    }
}
