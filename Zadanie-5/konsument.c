#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
void zamknij(int filee)
{
    if (close (filee) == -1) {
        perror ("Błąd zamknięcia!");
        exit (EXIT_FAILURE);
    }
}
int main (int argc, char *argv[]) {
    char buff[1024];
    int plik,potok,reading;
    if (argc != 3) {
        printf ("Błąd: niepoprawna liczba argumentów!\n");
        exit (EXIT_FAILURE);
    }
    if ((plik = open (argv[1], O_WRONLY | O_CREAT | O_TRUNC, 110100100)) == -1) {
        perror ("Błąd pliku");
        exit (EXIT_FAILURE);
    }

    if ((potok = open (argv[2], O_RDONLY, 110100100)) == -1) {
        perror ("Błąd pliku");
        exit (EXIT_FAILURE);
    }

    // zapisywanie skosumowanych produktów do pliku
    for(;;){
        sleep (1);
        reading = read(potok, &buff, 8);
        if (reading == -1) {
            printf("Błąd odczytu z potoku\n");
            exit(EXIT_FAILURE);
        } else if (reading == 0) {
            break;
        } else {
            if (write (plik, &buff, reading) == -1){
                perror("Błąd wpisania do pliku");
                exit(EXIT_FAILURE);
            }
            buff [reading] = '\0';
            printf("Konsumuje: %s\n",buff);
        }
    }
    zamknij(plik);
    zamknij(potok);
    return 0;
}