#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
pthread_t tid[3];
pid_t main_pid;
int job_num=0;
pid_t job_list[100];
void sighandler(int signum){

    // printf("child notice !\n");
    kill(main_pid , SIGUSR1);
    exit(0);
}
void sighandler_usr(int signum){

    printf("RINGGGGG !\n");
}

void setAlarm( int times){
    pid_t pid;
    printf("alarm set [%d]\n",times);
    pid = fork();
    if(pid > 0){
        job_list[job_num++] = pid;
    }
    if(pid == 0){

        signal (SIGALRM, sighandler);
        alarm(times);
        while(1){
        }
    }
}

void clearAlarm(){
    int i;
    for(i=0;i < job_num ;i++){
        kill(job_list[i],SIGTERM);
    }
    job_num = 0;
    printf("clear all exist alarm!\n");
    
}

int main(int argc, char *argv[]){


    int i = 0;
    int err;
    main_pid = getpid();
    signal (SIGUSR1, sighandler_usr);


    /* test case*/
    setAlarm(2);
    sleep(1);
    setAlarm(6);
    sleep(1);    
    setAlarm(3);
    sleep(4);
    clearAlarm();
    /* test case*/

    while(1){}
    return 0;
}