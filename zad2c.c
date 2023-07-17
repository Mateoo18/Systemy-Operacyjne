#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
void nowyproces(char *argv[]){
    if(!fork()){
        if((execlp("./zad2a.x", "./zad2a.x", argv[1],argv[2], (char *) NULL))==-1)
        {
            perror("execlp error");
            _exit(2);
        }
    }
    else
    {
    }
    
}
void ignoreHandler(int signum){

}

int main(int argc,char *argv[]) {
    if (argc != 3)
    {
        perror("Niewłaściwa liczba argumentów. Prawidłowy format: ./a.out [opcja] [nr sygnału]");
        exit(EXIT_FAILURE);
    }

    int childPID = fork();
    if(childPID == -1){
        printf("Fork error\n");
        exit(1);
    }
    if(!childPID){
    printf("%d\n",getpid());
        setpgid(getpid(),getpid());
        if(signal(atoi(argv[2]),ignoreHandler) == -1){
            printf("Signal error\n");
            exit(2);
        }
        for(int i=0;i<3;i++){
            if(!fork()) {
                nowyproces(argv);
            }
            else{
            wait(NULL);
        
            }
        }
    }
    else{
       sleep(1);
        if(killpg(childPID,atoi(argv[2]))==-1)
        {
        printf("Kill error!\n");
        exit(1);
        }
        
        int stat;
        wait(NULL);

    }
    
    return EXIT_SUCCESS;
}
