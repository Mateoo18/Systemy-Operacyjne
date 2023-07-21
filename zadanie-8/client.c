#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <mqueue.h>
#include "lib.h"

mqd_t serwer;

void wyjscie(void) {
    kolejka_zamknij (serwer);
}

void sygnal_wyjscie(int signal) {
    printf ("\nSygnał przerwania, zamykanie kolejki\n");
    exit(EXIT_SUCCESS);
}

int main () {

    char kolejka_odpowiedz_nazwa[10],odczyt[LENGTH],string[LENGTH+10];
    mqd_t kolejka_odpowiedz;
    sprintf (kolejka_odpowiedz_nazwa, "/%d", getpid());
    sleep (2);
    serwer = kolejka_otworz (KOLEJKA_NAZWA, O_WRONLY);
    if (atexit (wyjscie) != 0) {
        perror("atexit error");
        _exit(EXIT_FAILURE);
    }

    if (signal (SIGINT,sygnal_wyjscie) == SIG_ERR) {
        perror("Błąd signal");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));
    atributes(serwer);
    sleep(((double)rand()) / RAND_MAX);

    while (1) {

        printf ("Wpisz działanie ktore chcesz obliczyc\n");
        if (scanf ("%s", odczyt) == EOF) {

            printf ("Klient zamkniety\n");
            exit(EXIT_SUCCESS);
        }
        sprintf(string, "%d %s", getpid(), odczyt);
        kolejka_wyslij(serwer, string, 1);
        printf ("\nWysłano  do serwera: %s\n", string);
        kolejka_odpowiedz = kolejka_utworz (kolejka_odpowiedz_nazwa, O_RDONLY);
        printf ("Oczekiwanie na odpowiedź z serwera\n");
        kolejka_odbierz (kolejka_odpowiedz, string, NULL);
        printf ("Odpowiedź z serwera: %s\n\n", string);
        kolejka_zamknij (kolejka_odpowiedz);
        kolejka_usun (kolejka_odpowiedz_nazwa);

        sleep(((double)rand()) / RAND_MAX);
    }

}