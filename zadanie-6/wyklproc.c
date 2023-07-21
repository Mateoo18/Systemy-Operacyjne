#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include "semfunk.h"
#include <time.h>
#include <fcntl.h>
#include <string.h>
void errorplik()
{
    perror ("Błąd pliku");
    exit (EXIT_FAILURE);
}
int main (int argc, char *argv[]) {
    int odczyt,liczba,c_inre,val_sem,plik;
    char bufor[100];
    sem_t *semafor;

    if (argc != 4) {
        printf ("Błąd: niepoprawna liczba argumentów!\n");
        _exit (EXIT_FAILURE);
    }
    c_inre = atoi(argv[1]);

    semafor = opensem(argv[3]);

    for (int i=0;i<c_inre;i++) {

        srand(time(NULL));

        sleep (1.5+rand()%2);

            valsem(semafor, &val_sem);
            int pidd=getpid();
            printf ("PID procesu %d - wartosc semafora przed wejsciem w niego: %d\n", pidd, val_sem);
            levsem(semafor);//wait
//critical
        if ((plik = open (argv[2], O_RDONLY, 0644)) == -1) {
          errorplik();
        }
        odczyt = read(plik, &bufor, 10);//przeczytaj to co jest w pliku
        if (odczyt == -1 || odczyt==0) {
           errorplik();
        } else {
            bufor [odczyt] = '\0';
            valsem(semafor, &val_sem);
            printf("!!!!!!!   PID procesu %d -- w critical semafor ma wartosc %d , a w pliku tekstowym %s\n ",pidd, val_sem, bufor);
        }
        if (close (plik) == -1) {
           errorplik();
        }

        liczba = atoi (bufor);
        liczba++;
        sprintf (bufor, "%d", liczba);//zapisanie do bufora liczbe
        printf ("liczba wpisywana do pliku %d\n", liczba);
        sleep (1+rand()%3);

        if ((plik = open (argv[2], O_WRONLY |  O_TRUNC, 0644)) == -1) {
            errorplik();
        }

        if (write (plik, &bufor, 1) == -1){
            errorplik();
        }

        if (close (plik) == -1) {
            errorplik();
        }
            incsem(semafor);//post
            valsem(semafor, &val_sem);
            printf ("PID danego procesu %d po sekcji krytycznej semafor ma wartosc %d\n", getpid(), val_sem);
    }

        closesem(semafor);
    return 0;
}