#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>

pthread_t tid[3];
void sighandler1(int signum){
   printf("Signal handling thread 1 got signal %d\n",signum);
   kill(getpid() , SIGCONT);
}
void sighandler2(int signum){
   printf("Signal handling thread 2 got signal %d\n",signum);
   exit(1);
   printf("exit?\n");
}
void sighandler3(int signum){
   printf("Signal handling thread 3 got signal %d\n",signum);

}
void* doSomeThing(void *arg){
    unsigned long i = 0;
    pthread_t id = pthread_self();
    // signal (SIGUSR1, SIG_IGN);
    // signal (SIGINT, SIG_IGN);
	signal (SIGQUIT, SIG_IGN);
	signal (SIGTSTP, SIG_IGN);
	signal (SIGTTIN, SIG_IGN);
	signal (SIGTTOU, SIG_IGN);
	signal (SIGCHLD, SIG_IGN);

    if(pthread_equal(id,tid[0]))
    {
        printf("\n First thread processing\n");
        signal (SIGINT, sighandler1);
        kill(getpid() , SIGCONT);

    }
    else if(pthread_equal(id,tid[1])){
        printf("\n Second thread processing\n");
        signal (SIGTERM, sighandler2);
    }
    else if(pthread_equal(id,tid[2])){
        printf("\n third thread processing\n");
        signal (SIGUSR1, sighandler3);
    }

    while(1){}
    printf("NO LINE!\n");

    return NULL;
}

int main(void){
    int i = 0;
    int err;
    
    while(i < 3){
        err = pthread_create(&(tid[i]), NULL, &doSomeThing, NULL);
        if (err != 0)
            printf("\ncan't create thread :[%s]", strerror(err));
        else
            printf("\n Thread created successfully\n");

        i++;
    }

    while(1){}
    printf("No line!\n");
    return 0;
}