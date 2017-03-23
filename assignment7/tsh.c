#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <signal.h>



typedef struct list{
	int pid;
	char name[200];
}PROC;

char input[1000];
char arg[1000];
char *input_parse;
int k=0;
int invisible = 0;
int job_num=0;   //job numbers
int terminal_pid;
PROC job_list[1024];  // job data;

void sighandler(int signum){
   printf("catch\n");
   exit(1);
}

int main(int argc, char **argv){
	char buf[80];
	pid_t pid;
	//char*ps_argv[]={"ps","a", NULL};
	int a,i;
	char*ps_argv[100];
	char cmd[100];

	signal (SIGINT, SIG_IGN);
	signal (SIGQUIT, SIG_IGN);
	signal (SIGTSTP, SIG_IGN);
	// signal (SIGTTIN, SIG_IGN);
	signal (SIGTTOU, SIG_IGN);
	// signal (SIGCHLD, SIG_IGN);
	tcsetpgrp (0, getpgrp());/* Grab control of the terminal.  */
	terminal_pid = getpgrp();
	job_num=0;
	printf("grp: %d || tcgrp: %d \n",getpgrp(),tcgetpgrp(0));
	while(1){
		getcwd(buf, sizeof(buf));
    	//printf("%s:  ", buf);
		printf("$");
		fgets(input,1000,stdin);


		for(i=0;i<job_num;i++){
			if(kill(job_list[i].pid ,0)==0)
				waitpid(job_list[i].pid,NULL,WNOHANG);
		}
		
		input_parse = strtok(input , " \n");
		if(input_parse == NULL){
			continue;
		}

		if(strcmp(input_parse, "pwd") == 0){
    		getcwd(buf, sizeof(buf));
    		printf("%s\n", buf);
		}else if(strcmp(input_parse , "exit")==0 ){
			exit(0);
		}else if (strcmp(input_parse , "cd")==0 ){
			input_parse = strtok(NULL , " \n");
			chdir(input_parse);
		}else if (strcmp(input_parse , "fg")==0 ){

			signal (SIGTTOU, SIG_IGN);
			input_parse = strtok(NULL , " \n");
			int foreground  = atoi(input_parse);
			foreground = job_list[foreground-1].pid;
			if(tcsetpgrp (STDIN_FILENO, foreground) == -1)
				printf("ERR while set TC\n");
			
			printf("current tc in ID:%d\n",tcgetpgrp(0));
			printf("current tc ou ID:%d\n",tcgetpgrp(1));
			kill(foreground , SIGCONT);
			waitpid(foreground , NULL , 0);
			tcsetpgrp (STDIN_FILENO, terminal_pid);
			printf("#current tc in ID:%d\n",tcgetpgrp(0));
			printf("#current tc ou ID:%d\n",tcgetpgrp(1));
			kill(terminal_pid , SIGCONT);

		}else if(strcmp(input_parse , "jobs")==0 ){
			for(i=0;i<job_num;i++){
				if(kill(job_list[i].pid ,0)==0)
					printf("job %d : %s  pid:%d\n",i+1,job_list[i].name,job_list[i].pid);
			}
		}
		else {
			k=0;
			invisible = 0;
			strcpy(cmd,input_parse);
			ps_argv[k] = malloc(strlen(cmd)+1);
			strcpy(ps_argv[k++],cmd);
			
			char tmp[100];
			
			while((input_parse = strtok(NULL, " \n")) && input_parse != NULL){
				if(strcmp(input_parse,"&")==0){
					invisible = 1;
					continue;
				}
				ps_argv[k] = malloc(strlen(input_parse)+1);
				strcpy(ps_argv[k++],input_parse);
			}
			ps_argv[k] = malloc(1);
			ps_argv[k] = NULL;
			if(invisible == 1){
				pid = fork();
				if(pid > 0){  // add job to list	
					strcpy(job_list[job_num].name,cmd);
					job_list[job_num++].pid = pid;
					//printf("add %d to list\n",pid);
				}
				
				if(pid == 0){
					printf("invisible proc pid:%d  PGID:%d\n",getpid(),getpgrp());
					if(setpgid(getpid(),getpid())==0)
						printf("set\n");
					printf("invisible proc pid:%d  PGID:%d\n",getpid(),getpgrp());
					printf("parent ID: %d\n",getppid());
				}
			}else{
				pid = fork();
				if(pid > 0){  // add job to list
					strcpy(job_list[job_num].name,cmd);
					job_list[job_num++].pid = pid;
					//printf("add %d to list\n",pid);
				}
				if(pid != 0)
					waitpid(pid,NULL,0);
			}	
			if(pid == 0){
					int t;
					signal (SIGINT, sighandler);
					signal (SIGQUIT, SIG_DFL);
					signal (SIGTSTP, SIG_DFL);
					signal (SIGTTIN, SIG_DFL);
					signal (SIGTTOU, SIG_DFL);
					signal (SIGCHLD, SIG_DFL);
					t = execvp(cmd,ps_argv);
					if(t < 0)
						printf("%s: command not found\n",cmd);
					exit(0);
			}	
		}


		
	}
	

	return 0;
}