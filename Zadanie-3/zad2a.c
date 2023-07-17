#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void my_sighandler(int signal)
{
    printf("Poprawnie przechwycono sygnał %d\n", signal);
 // printf( "HALO Halo: %s\n",strsignal((signal >> 8) & (255))) ;
  printf(" hej PID: %d\n",getpid()); 
  //printf( "HALO Halo: %s\n",strsignal((signal >> 8) & (255))) ;
  pause();
 exit(signal);
   
}

int procinfOO(const char *name)
{
    int pgid = getpgid(getpid());
    if(pgid < 0)
    {
        perror("Wystąpił bład podczas odczytu PGID\n");
    }

    printf("Nazwa programu: %s, UID: %i, GID: %i, PID: %i, PPID: %i, PGID: %i\n",
           name, getuid(), getgid(), getpid(), getppid(), pgid);
    return 0;
}

int main(int argc, char **argv) {

    if (argc != 3)
    {
        perror("Niewłaściwa liczba argumentów. Prawidłowy format: ./a.out [opcja] [nr sygnału]");
        exit(EXIT_FAILURE);
    }
    
int pgid = getpgid(getpid());
    
    procinfOO(argv[0]);
    char k=*argv[1];
    int signal_number = atoi(argv[2]);
    __sighandler_t signal_call;
    switch (k)
    {
        case 'd':
            signal_call = signal(signal_number, SIG_DFL);
            
            if (signal_call == SIG_ERR)
            {
                perror("Wystąpił błąd domyślnej obsługi sygnału");
                exit(EXIT_FAILURE);
            }
            break;
        case 'i':

            signal_call = signal(signal_number, SIG_IGN);
            if (signal_call == SIG_ERR)
            {
                perror("Wystąpił błąd ignorowania sygnału");
                exit(EXIT_FAILURE);
            }
            else
            {
            printf("Zignorowano sygnal!\n");
            }
            break;
        case 'p' :
       // printf("JESTEM TUTAJ\n");
            signal_call = signal(signal_number, my_sighandler);
            if (signal_call == SIG_ERR)
            {
                perror("Wystąpił błąd przechwytywania sygnału");
                exit(EXIT_FAILURE);
            }
            else{
            //my_sighandler(signal_number);
            }

            break;
        default:
        pause();
            printf("Wybrana opcja nie istnieje! Możliwe opcje: d, i, p\n");
            break;
    }
    printf("PID: %d\n",getpid());  
    
     pause();
    printf("Zakończono działanie procesu.\n");
   // printf( "HALO Halo: %s\n",strsignal((signal_number >> 8) & (255))) ;
    exit(signal_number);
 
}
