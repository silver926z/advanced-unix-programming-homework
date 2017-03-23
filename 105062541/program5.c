#include <dirent.h>
#include <limits.h>
#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char *argv[]){
	pid_t pid;
	if ((pid = fork()) < 0) {
		fprintf(stderr,"fork error");
	}else if(pid == 0){
		setsid();
		printf("child group ID: %d\n",getpgrp());
		printf("child process ID: %d\n",getpid());
		exit(0);
	}
	sleep(2);
	if (waitpid(pid, NULL, 0) != pid) /* wait for first child */
       printf(stderr,"wait pid error");	

    printf("parent group ID: %d\n",getpgrp());
	printf("parent process ID: %d\n",getpid());



	return 0;
}