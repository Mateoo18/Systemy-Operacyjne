#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>
char potok[20];

void wyjscie(void) {
    //printf("HALO");
    unlink (potok);
}

int main (int argc, char *argv[]) {
    strcpy (potok, argv[5]);
    if (argc != 6) {
        printf ("Niepoprawna liczba argumentów!\n");
        _exit (EXIT_FAILURE);
    }

    if (mkfifo(argv[5], 0644) == -1) {
        perror("Błąd potoku");
        _exit (EXIT_FAILURE);
    }
    int i=0;
while(i<2)
     {
int res=fork();
        if(res==0)
        {
            if (i == 0) {

                execl (argv[1], argv[1], argv[3], argv[5], NULL); // urucham program producenta
            } else {

                execl (argv[2], argv[2], argv[4], argv[5], NULL); // urucham program konsumenta
            }
        }
        else if(res==-1)
        {
            perror("fork error");
            exit(EXIT_FAILURE);
        }

i++;
    }
wait(NULL);
    wait(NULL);
    if (atexit (wyjscie) != 0) {
        perror("atexit error");
        _exit(EXIT_FAILURE);
    }
    return 0;

}