#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include "lib/pamdziel.h"
#include "lib/semafor.h"
#include <string.h>
#define NELE 10
#define NBUF 5

typedef struct {
    char bufor[NBUF][NELE]; // Wspolny bufor danych
    int zapis;
    int odczyt;
} SegmentPD;

int main (int argc, char *argv[]) {

    int plik,PD_desk,odczyt;
    SegmentPD *PD;
    sem_t *semafor_producent;
    sem_t *semafor_konsument;
    if (argc != 5) {
        printf ("Błąd: niepoprawna liczba argumentów!\n");
        exit (EXIT_FAILURE);
    }

    if ((plik = open (argv[4], O_RDONLY, 0644)) == -1) {
        printf ("Błąd otwarcia pliku %s do odczytu\n", argv[4]);
        perror ("Błąd pliku");
        exit (EXIT_FAILURE);
    }

    semafor_producent = otworz_semafor (argv[1]);
    printf ("Producent: otwarto semafor producenta %s deskryptor %p\n", argv[1], (void *)semafor_producent);
    semafor_konsument = otworz_semafor (argv[2]);
    printf ("Producent: otwarto semafor konsumenta %s deskryptor %p\n", argv[2], (void *)semafor_konsument);

    PD_desk = otworz_PD (argv[3]);

    printf ("Producent: pamięć dzielona %s deskryptor %d\n", argv[3], PD_desk);

    PD = (SegmentPD *) odwzoruj_PD (PD_desk, sizeof(SegmentPD));
    PD->zapis = 0;

    while (1) {
        sleep(2);
memset(PD->bufor[PD->zapis],'\0',NELE+1);
        printf ("Producent: przed SK semafor: %d\n", wartosc_semafor(semafor_producent));
        opusc_semafor (semafor_producent);

        odczyt = read(plik, PD->bufor[PD->zapis], NELE);
        if (odczyt == -1) {
            printf("Błąd odczytu z pliku\n");
            exit(EXIT_FAILURE);
        } if (odczyt != NELE) {
            PD->bufor[PD->zapis][odczyt] = '\0';

            printf ("\tProdukuje =>  %s\n",  PD->bufor[PD->zapis]);
            podnies_semafor (semafor_konsument);
            printf ("Producent: po SK semafor: %d\n", wartosc_semafor (semafor_konsument));
            break;
        } else {
            printf ("\tProdukuje =>  %s\n",  PD->bufor[PD->zapis]);
            printf ("\n\tIndeks bufora zapis: %d\n", PD->zapis);
        }
        PD->zapis = (PD->zapis + 1) % NBUF;
        podnies_semafor (semafor_konsument);
        printf ("Producent: po SK semafor: %d\n", wartosc_semafor (semafor_konsument));
    }

    if (close (plik) == -1) {
        perror ("Błąd zamknięcia pliku wyjścia");
        exit (EXIT_FAILURE);
    }
    usun_odwzorowanie_PD(PD, sizeof(SegmentPD));
    zamknij_PD (PD_desk);
    zamknij_semafor(semafor_producent);
    zamknij_semafor(semafor_konsument);
    printf ("Producent: koniec procesu\n");
    return 0;
}