#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <mqueue.h>
#include "lib.h"

void wyjscie(void) {
    kolejka_usun (KOLEJKA_NAZWA);
}

void sygnal_wyjscie(int signal) {
    printf ("\nSygnał przerwania, usuwanie kolejki\n");
    exit(EXIT_SUCCESS);
}

int main () {
    //kolejka_usun(KOLEJKA_NAZWA);
    int PID_klienta,liczba1,liczba2,wynik,czy_err=1;
    char wynik_s[LENGTH], string[LENGTH],operator,err[LENGTH+5];
    mqd_t kolejka_desk,odpowiedz;

    kolejka_desk = kolejka_utworz (KOLEJKA_NAZWA, O_RDONLY);
atributes(kolejka_desk);

    if (atexit (wyjscie) != 0) {
        perror("atexit error");
        exit(EXIT_FAILURE);
    }

    if (signal (SIGINT,sygnal_wyjscie) == SIG_ERR) {
        perror("Błąd signal");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));

    while (1) {
        kolejka_odbierz (kolejka_desk, string, NULL);
        sscanf(string, "%d %d%c%d", &PID_klienta, &liczba1, &operator, &liczba2);//formatuje zmienna string i wpisuje odpowiedno do danych zmiennnych
        printf ("Pytanie od PID: %d nalezy rozwiazac rownanie: %d %c %d\n", PID_klienta, liczba1, operator, liczba2);

        switch(operator)
        {
            case '+':
                wynik = liczba1 + liczba2;
                sprintf (wynik_s, "%d", wynik);
                break;

            case '-':
                wynik = liczba1 - liczba2;
                sprintf (wynik_s, "%d", wynik);
                break;

            case '*':
                wynik = liczba1 * liczba2;
                sprintf (wynik_s, "%d", wynik);
                break;
            case '/':
                if(liczba2!=0) {
                    wynik = liczba1 / liczba2;
                    sprintf(wynik_s, "%d", wynik);
                }
                else{
                    sprintf(err,"NIE WOLNO DZIELIC PRZEZ ZERO!\n");
                czy_err=0;
                }
                break;
            default:
                sprintf (err, "Nieznany znak!\n");
                czy_err=0;
        }
        sleep(((double)rand()) / RAND_MAX);
char help[LENGTH];
        sprintf (help, "/%d", PID_klienta);
        odpowiedz = kolejka_otworz (help, O_WRONLY);
        if(czy_err==1) {
            printf("Wysyłanie wyniku: %s do %d\n", wynik_s, PID_klienta);
            kolejka_wyslij(odpowiedz, wynik_s, 1);
        }
        else
        {
            printf("Wysyłanie wyniku: %s do %d\n", err, PID_klienta);
            kolejka_wyslij(odpowiedz, err, 1);
            czy_err=1;
        }
        kolejka_zamknij (odpowiedz);
    }

}