#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include "semfunk.h"
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>

char namesem[20];
void wyjscie(void) {

    erasem(namesem);
}
void sygnal_wyjscie(int signal) {

        printf ("Sygnał przerwania, usuwanie semafora\n");
        erasem(namesem);
    _exit(EXIT_SUCCESS);
}
void errorplik()
{
    perror ("Błąd pliku");
    exit (EXIT_FAILURE);
}

void plikaction(char *stro)
{
    int plik;
    if ((plik = open (stro, O_WRONLY | O_TRUNC, 0644)) == -1) {
        errorplik();
    }

    if (write (plik, "0", 2) == -1){
        errorplik();
    }
    if (close (plik) == -1) {
        errorplik();
    }
}
int main (int argc, char *argv[]) {
int  countincre,plik,odczyt,val_sem;
strcpy(namesem,argv[5]);
    if (argc != 6) {
        printf ("niepoprawna liczba argumentów!\n");
        exit (EXIT_FAILURE);
    }
    if (signal (SIGINT, sygnal_wyjscie) == SIG_ERR) {
        perror("Błąd signal");
        exit(EXIT_FAILURE);
    }
    if (atexit (wyjscie) != 0) {
        perror("atexit error");
        exit(EXIT_FAILURE);
    }
    plikaction(argv[4]);
    countincre = atoi (argv[2]);

        sem_t *semafor = createsem(argv[5]);
        valsem (semafor, &val_sem);
       printf ("SEMAFOR:  (%p), wartosc:  %d\n",  (void *)semafor, val_sem);

    for (int i=0;i<countincre;i++) {
        switch (fork ()) {
            case -1:
                perror("fork error");
                exit(EXIT_FAILURE);
            case 0:
                execl (argv[1], argv[1], argv[3], argv[4], argv[5], NULL);
           break;
            default:
                break;
        }
    }
    for (int i=0;i<countincre;i++) {
        wait(NULL);
    }
    if ((plik=open(argv[4], O_RDONLY, 0644)) == -1) {
       errorplik();
    }
    char bufor[10];
    odczyt = read(plik, &bufor, 10);
    int procesy=atoi(argv[2]);
    int sekcje=atoi(argv[3]);
    int liczba_w_bufor=atoi(bufor);
    if (odczyt == -1 || odczyt==0) {
        errorplik();
    }
    else {
        bufor [odczyt] = '\0';
        if (liczba_w_bufor == procesy*sekcje) {
            printf ("\nsynchronizacja udana- liczba : %d\n", liczba_w_bufor);
        } else {
            printf ("\nsynchronizacja nieudana- liczba %d",liczba_w_bufor);
        }
    }
    if (close (plik) == -1) {
       errorplik();
    }
    return 0;
}