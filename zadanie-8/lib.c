#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include "lib.h"

mqd_t kolejka_utworz (const char *name, int oflag) {
    struct mq_attr attr;
    attr.mq_maxmsg = SIZEKOM;
    attr.mq_msgsize = LENGTH;
    mqd_t des = mq_open(name, O_CREAT | O_EXCL | oflag, 0644, &attr);
    if (des == -1) {
        perror ("błąd tworzenia kolejki");
        exit (EXIT_FAILURE);
    }
    return des;
}

mqd_t kolejka_otworz (const char *name, int oflag) {
    mqd_t des = mq_open(name, oflag);
    if (des == -1) {
        perror ("błąd otwierania kolejki");
        exit (EXIT_FAILURE);
    }
    return des;
}

void kolejka_wyslij (mqd_t mq_des, const char *msg_ptr, unsigned int msg_prio) {
    if (mq_send(mq_des, msg_ptr, LENGTH, msg_prio) == -1) {
        perror ("błąd wysyłania komunikatu");
        exit (EXIT_FAILURE);
    }
}

void kolejka_odbierz (mqd_t mq_des, char *msg_ptr, unsigned int *msg_prio) {
    if (mq_receive(mq_des, msg_ptr, LENGTH, msg_prio) == -1) {
        perror ("błąd odbierania komunikatu");
        exit (EXIT_FAILURE);
    }
}

void kolejka_zamknij (mqd_t mq_des) {
    if (mq_close(mq_des) == -1) {
        perror ("błąd zamykania kolejki");
        exit (EXIT_FAILURE);
    }
}

void kolejka_usun (const char *name) {
    if (mq_unlink(name) == -1) {
        perror ("błąd usuwania kolejki");
        exit (EXIT_FAILURE);
    }
}

struct mq_attr kolejka_pobierz_atrybuty (mqd_t mq_des) {
    struct mq_attr atr;
    if (mq_getattr(mq_des, &atr) == -1) {
        perror ("błąd pobierania atrybutów kolejki");
        exit (EXIT_FAILURE);
    }
    return atr;
}

void atributes(mqd_t que)
{
    struct mq_attr atrybuty = kolejka_pobierz_atrybuty(que);
    printf("Stworzono kolejke: %s o deskryptorze %d  \n", KOLEJKA_NAZWA, que);
    printf("Atrybuty kolejki: \n");
    printf("Ustawione flagi kolejki: = %ld\n", atrybuty.mq_flags);
    printf("Liczba komunikatow obecnie w kolejce: = %ld\n\n", atrybuty.mq_curmsgs);
    printf("Max komunikatow w kolejce: = %ld\n", atrybuty.mq_maxmsg);
    printf("Max rozmiar komunikatow: = %ld\n", atrybuty.mq_msgsize);
}