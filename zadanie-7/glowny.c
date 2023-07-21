#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include "lib/pamdziel.h"
#include "lib/semafor.h"

#define NELE 20 // Rozmiar elementu bufora (jednostki towaru) w bajtach
#define NBUF 5 // Liczba elementow bufora

  char NAZWAPD[20],sem_prod[20],sem_kon[20];

void wyjscie(void) {
    usun_PD (NAZWAPD);
    usun_semafor (sem_prod);
    usun_semafor (sem_kon);
}

void sygnal_wyjscie(int signal) {
    printf ("\nSignal Eroor\n");
    exit(EXIT_SUCCESS);
}

void create(sem_t *semm,int desk){
    semm = stworz_semafor (sem_prod, 5);
    printf ("Utworzono semafor producenta %s (%p) o wartości początkowej %d\n", sem_prod, (void *)semm, wartosc_semafor(semm));
    semm = stworz_semafor (sem_kon, 0);
    printf ("Utworzono semafor producenta %s (%p) o wartości początkowej %d\n", sem_kon, (void *)semm, wartosc_semafor(semm));
    desk = stworz_PD (NAZWAPD);
    ustaw_rozmiar_PD (desk, 108);
    printf ("Utworzono pamięć dzieloną %s o deskryptorze %d i rozmiarze %ld\n\n\n", NAZWAPD, desk, 108);
}


int main (int argc, char *argv[]) {
    strcpy(NAZWAPD,argv[7]);
    strcpy(sem_prod,argv[5]);
    strcpy(sem_kon,argv[6]);
   // usun_semafor(sem_prod);
    //usun_semafor(sem_kon);
   // usun_PD(NAZWAPD);
    int PD_desk;
    sem_t *semafor;
    if (argc <7) {
        printf ("Błąd: niepoprawna liczba argumentów!\n");
        exit (EXIT_FAILURE);
    }
    create(semafor,PD_desk);

    if (atexit (wyjscie) != 0) {
        perror("atexit error");
        _exit(EXIT_FAILURE);
    }

    if (signal (SIGINT,sygnal_wyjscie) == SIG_ERR) {
        perror("Błąd signal");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 2; i++) {
        switch (fork ()) {
            case -1:
                perror("fork error");
                exit(EXIT_FAILURE);
            case 0:
                if (i == 0) {
                    execl (argv[3], argv[3], sem_prod, sem_kon, argv[7], argv[1], NULL); // urucham program producenta
                    exit (EXIT_FAILURE);
                } else {
                    execl (argv[4], argv[4], sem_prod, sem_kon, argv[7], argv[2], NULL); // urucham program konsumenta
                    exit (EXIT_FAILURE);
                }

                break;

            default:
                break;

        }
    }
    wait(NULL);
    wait(NULL);
    zamknij_PD (PD_desk);
    printf ("\nKONIEC\n");
    return 0;
}