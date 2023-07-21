#define _REENTRANT
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "lib.h"
//#include <cstdlib>
int liczba, num_thr, il_pet;
pthread_t arr[6];
pthread_mutex_t mutex;
void ustaw_kursor (unsigned x, unsigned y) {
    printf ("\033[%d;%dH\033[2K", y, x);//033[ - To jest kod ucieczki ANSI, który rozpoczyna sekwencję sterującą.,  - Oznacza ustawienie kursora na określonej pozycji w terminalu. Wartości y i x zostaną podstawione na odpowiednie miejsca, aby przesunąć, \033[2K - To jest kod ucieczki ANSI, który czyszczenie linii terminala. 2K
}
void *watek_start (void *argument) {
    int licznik ,i=0;
    int numW = *(int *)argument;
    srand (time (NULL));

    printf ("Wątek nr: %d, ID: %ld \n", numW + 1, arr[numW]);

    sleep (1.9);
while(i<il_pet)
{
    i++;
        ustaw_kursor (0, 4 + num_thr + numW);
        printf("\033[33mNUMER WATKU: %d, liczba: %d \033[0m\n", numW + 1, liczba);
        sleep (1.8);
        close_mutex(&mutex);
        ustaw_kursor (50, 4 + num_thr + numW);
        printf("\033[1;34mNUMER WATKU: %d, liczba: %d \033[0m\n", numW + 1, liczba);
        licznik = liczba;
        licznik++;
        sleep(2);
        liczba = licznik;
        open_mutex (&mutex);
        ustaw_kursor (0, 4 + num_thr + numW);
        printf("\033[32mNUMER WATKUr: %d, liczba: %d \033[0m\n", numW + 1, liczba);
        sleep (2.5);
    }

}

int main (int argc, char *argv[]) {

    if (argc != 3) {
        printf ("Błąd: niepoprawna liczba argumentów!\n");
        exit (EXIT_FAILURE);
    }
    num_thr = atoi(argv[1]);
    il_pet = atoi(argv[2]);
    create_mutex (&mutex);
    printf("Zainicjalizowano mutex o adresie %p\n", (void *)&mutex);
    liczba = 0;

    for (int i=0; i<num_thr;i++) {

        int *mem=malloc (sizeof (int));
        *mem=i;
        sleep (0.5);
        arr[i] = create_thr(watek_start, (void *)mem);
        //free(mem);
    }
    for (int i = 0; i < num_thr; i++) {
        join_thr(arr[i]);
    }
    ustaw_kursor (0, 7 + num_thr * 2);
    int wynik=num_thr * il_pet;
    if (liczba != wynik) {
        printf ("Synchronizacja nie powiodła się!\n");

    } else {
        printf ("Synchronizacja powiodła się\n");
    }
    des_mutex (&mutex);
    return 0;
}