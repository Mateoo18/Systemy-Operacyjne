//
// Created by mateusz on 27.03.23.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>
extern const char * const sys_siglist[];
int main(int argc, char **argv) {
    int result;
    result = fork();
    int signal_x = atoi(argv[2]);
    if (result < 0) {
        perror("Bład wykonania funkcji fork()!\n");
        exit(EXIT_FAILURE);
    } else if (result == 0) { //potomny
       if( execlp("./zad2a.x", "./zad2a.x", argv[1], argv[2], NULL)==-1)
       {
       perror("execlp error");
            _exit(2);
       }

    } else { //macierzysty
        sleep(1);
        kill(result,0);
        if (errno != ESRCH)
        {
            kill(result,signal_x); 
            printf("wysłano sygnał do procesu %i.\n", result);
        }
        if(kill(result,atoi(argv[2])) == -1){
            printf("Kill error\n");
            exit(1);
        }
        int status;
        wait(NULL);      
    }

   // return EXIT_SUCCESS;
}

