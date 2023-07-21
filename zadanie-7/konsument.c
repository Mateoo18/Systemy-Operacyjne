#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include "lib/pamdziel.h"
#include "lib/semafor.h"

#define NELE 10 // Rozmiar elementu bufora (jednostki towaru) w bajtach
#define NBUF 5// Liczba elementow bufora

typedef struct {
    char bufor[NBUF][NELE]; // Wspolny bufor danych
    int zapis;
    int odczyt;
} SegmentPD;

int main (int argc, char *argv[]) {

    int plik,PD_desk;
    SegmentPD *PD;
    sem_t *semafor_producent;
    sem_t *semafor_konsument;
    if (argc != 5) {
        printf("Błąd: niepoprawna liczba argumentów!\n");
        exit(EXIT_FAILURE);
    }
    if ((plik = open (argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1) {
        printf ("Błąd otwarcia pliku %s do zapisu", argv[4]);
        perror ("Błąd pliku");
        exit (EXIT_FAILURE);
    }

    semafor_producent = otworz_semafor (argv[1]);
    printf ("Konsument: otwarto semafor producenta %s deskryptor %p\n", argv[1], (void *)semafor_producent);
    semafor_konsument = otworz_semafor (argv[2]);
    printf ("Konsument: otwarto semafor konsumenta %s deskryptor %p\n", argv[2], (void *)semafor_konsument);

    PD_desk = otworz_PD (argv[3]);
    printf ("Konsument: pamięć dzielona %s deskryptor %d\n", argv[3], PD_desk);

    PD = (SegmentPD *) odwzoruj_PD (PD_desk, sizeof(SegmentPD));

    PD->odczyt = 0;

    int loop=1,counter=0,writing;
    while(loop)
    {
        counter=0;
        memset(PD->bufor[PD->odczyt],'\0',NELE);
        printf("wartosc semafora w konsument przed SK: %d\n", wartosc_semafor(semafor_konsument));
        opusc_semafor(semafor_konsument);

       writing= write(plik,PD->bufor[PD->odczyt], strlen(PD->bufor[PD->odczyt]));
        //printf("writing buff liczba: %d\n",writing);
        printf("indeks bufora odczyt: %d\n",PD->odczyt);
       if(writing==-1){
           printf("Błąd odczytu z pliku\n");
           exit(EXIT_FAILURE);
       }
        if(writing!=NELE){
            printf("zapisane dane: %s\n",PD->bufor[PD->odczyt]);
            printf("KONIEC");
            break;
        }
        printf("zapisane pliki: %s\n",PD->bufor[PD->odczyt]);
        PD->odczyt=(PD->odczyt+1)%NBUF;
        podnies_semafor(semafor_producent);
        printf("wartosc semafora w konsument po SK: %d\n", wartosc_semafor(semafor_konsument));
    }

    if (close (plik) == -1) {
        perror ("Błąd zamknięcia pliku wyjścia");
        exit (EXIT_FAILURE);
    }

    usun_odwzorowanie_PD(PD, sizeof(SegmentPD));
    zamknij_PD (PD_desk);
    zamknij_semafor(semafor_konsument);
    zamknij_semafor(semafor_producent);
    printf ("Konsument: koniec procesu\n");

    return 0;
}