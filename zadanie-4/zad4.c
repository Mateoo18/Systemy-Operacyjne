#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <time.h>
void zamknij(int filee)
{
    if (close (filee) == -1) {
        perror ("Błąd zamknięcia!");
        exit (EXIT_FAILURE);
        
    }
}
int main (int argc, char *argv[]) {
    char buff[1024];
    int plik,reading;
    int filedes[2];
    if (argc != 3) {
        printf ("Błąd: niepoprawna liczba argumentów!\n");
        exit (EXIT_FAILURE);
    }
    if (pipe(filedes) == -1) {
        perror("Błąd potoku");
        exit (EXIT_FAILURE);
    }
    srand(time(0));
    switch (fork ()) {
        case -1:
            perror("BLAD fork");
            exit(EXIT_FAILURE);
        case 0://konsument
            zamknij(filedes[1]);//zapis
            if ((plik = open (argv[2], O_WRONLY | O_TRUNC, 110100100)) == -1) {
                perror ("Błąd pliku");
                exit (EXIT_FAILURE);
            }
            for(;;){
                sleep(1.5+rand()%2);
                reading = read(filedes[0], &buff, 10);
                if(reading!=0 &&  reading!=-1){
               write (plik, &buff, reading);
                   buff [reading] = '\0';
                    printf("Konsumuje: %s\n",buff);
               }
               else if(reading==0)
               {
               break;
               }
                else if (reading == -1) {
                    printf("Błąd odczytu z potoku\n");
                    exit(EXIT_FAILURE);
                } 
            }
            zamknij(plik);
            zamknij(filedes[0]);
            break;
        default:
            zamknij(filedes[0]);
            if ((plik = open (argv[1], O_RDONLY, 110100100)) == -1) {
                perror ("Błąd pliku");
                exit (EXIT_FAILURE);
            }
            for(;;){
                sleep (1+rand()%3);
                reading = read(plik, &buff, 20);
                 if(reading!=0&&reading!=-1)
               {
                write (filedes[1], &buff, reading);
                    buff [reading] = '\0';
                    printf("Produkuje: %s\n",buff);
               }
               else if(reading==0)
               {break;}
               
                else if (reading == -1) {
                    printf("Błąd odczytu z pliku\n");
                    exit(EXIT_FAILURE);
                } 
            }
            zamknij(plik);
            zamknij(filedes[1]);
wait(NULL);
            break;
    }
    return 0;
}
