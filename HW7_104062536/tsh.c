#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

#define BUFSIZE 4096
char line_buf[BUFSIZE];
void remove_job_list(pid_t pid);
pid_t shell_gid;
void err_sys(char *str){
	//fprintf(stderr , "%s" , str);
	printf("%s",str);
}
static void sig_child(int signo){
	if(signo == SIGCHLD){
		//printf("child terminate\n");
		pid_t pid = waitpid(0,NULL,WNOHANG);
		remove_job_list(pid);
	}
	signal(SIGCHLD, sig_child);
}
static void sig_handler(int signo) { /* argument is signal number */
	if (signo == SIGINT){
		//printf("received SIGINT\n");
		signal(SIGINT , sig_handler);
	}else if (signo == SIGTSTP){
		//printf("received SIGTSTP\n");
		signal(SIGTSTP , sig_handler);
	}else if(signo == SIGTTOU){
		//printf("received SIGTTOU\n");
		signal(SIGTTOU , sig_handler);
	}
	printf("\n$ ");
}
void runCommand(pid_t pid ,char **argv ,int isBackground , char *command_str){
	if((pid = fork()) < 0){ 
		err_sys("fork error\n"); 
	}else if(pid == 0){
//		if(!fork()){
		//child
		setpgid(0, 0);
		signal (SIGINT, SIG_DFL);
		signal (SIGQUIT, SIG_DFL);
		signal (SIGTSTP, SIG_DFL);
		signal (SIGTTOU, SIG_DFL);
		signal (SIGCHLD, sig_child);
		if (execvp(argv[0],argv) < 0){
			printf("%s: command not found\n",argv[0]);
		}
//		}else{
//			exit(0);
//		}
		exit(0);
	}else{
		//parent
		add_job_list(pid , command_str);
		if(isBackground == 0){
			if(waitpid(pid , NULL , 0) < 0){
				{ err_sys("wait error\n"); }
			}
		}else{
			if(waitpid(pid , NULL , WNOHANG) < 0){
				{ err_sys("wait(non blocking) error\n"); }
			}
		}
	}
	
}
struct JNode{
	int id;
	int status;
	char command[1024];
	pid_t pid;
	struct JNode *next;
}job_list[1024];
int cur_id = 0;
void init_joblist(){
	int i;
	for(i = 0 ; i < 1024 ; i++){
		job_list[i].id = i;
		job_list[i].status = 0;
	}
}
void add_job_list(pid_t pid , char *command_str){
	job_list[cur_id].status = 1;
	job_list[cur_id].pid = pid;
	strcpy(job_list[cur_id].command , command_str);
	cur_id ++;
}
void remove_job_list(pid_t pid){
	int i;
	for(i = 0 ; i < 1024 ; i++){
		if(job_list[i].status == 1 && job_list[i].pid == pid){
			job_list[i].status = 2;
			break;
		}
	}
}

int main(){
	pid_t pid = 0;
	int argc;
	char **argv = malloc(sizeof(char *)*256);
	char *ptr;
	int isbg = 0;
	init_joblist();
	shell_gid = getpgrp();
	//signal(SIGINT, sig_handler);
	//signal(SIGTSTP , sig_handler);
	//signal(SIGTTOU , sig_handler);
	signal (SIGINT, SIG_IGN);
	signal (SIGQUIT, SIG_IGN);
	signal (SIGTSTP, SIG_IGN);
	//signal (SIGTTIN, SIG_IGN);
	signal (SIGTTOU, SIG_IGN);
	//signal (SIGCHLD, SIG_IGN);
	signal(SIGCHLD, sig_child);
	printf("$ ");
	while(fgets (line_buf, BUFSIZE, stdin)){
		isbg = 0;
		//printf("%s",line_buf);
		int size =strlen(line_buf);
		line_buf[--size] = '\0';
		char command_str[1024];
		strcpy(command_str , line_buf);
		if(line_buf[size-1] == '&'){
			line_buf[--size] = '\0';
			isbg = 1;
		}
		argc = 0;
		ptr = strtok(line_buf , " ");
		if(ptr != NULL){
			do{
				argv[argc++] = ptr;
			}while((ptr = strtok(NULL , " ")) != NULL);
		}else{
			printf("CHECK one argu!!\n");
			argc = 1;
			argv[0] = line_buf;
			//argv[1] = NULL;
		}
		argv[argc] = NULL;
		/*test
		int i;
		for (i = 0 ; i < argc ; i++){
			printf("argv[%d] : %s\n",i,argv[i]);
		}
		test*/
		
		int i;
		for (i = 0 ; i < cur_id ; i++){
			if(job_list[i].status == 1)
				waitpid(job_list[i].pid , NULL , WNOHANG);
		}
		if(strcmp(argv[0] , "cd") == 0){
			chdir(argv[1]);
		}else if(strcmp(argv[0] , "exit") == 0){
			exit(0);
		}else if(strcmp(argv[0] , "Jobs") == 0 || strcmp(argv[0] , "jobs") == 0){
			//argv[0] = "ps";argv[1] = "aux";argv[2] = NULL;argc = 2;
			//argv[0] = "ps";argv[1] = NULL;argc = 1;
			//runCommand(pid ,argv ,isbg);
			int i;
			for(i = 0 ; i < cur_id ; i++){
				if(job_list[i].status != 0){
					if(0 == kill(job_list[i].pid , 0)){
						if(job_list[i].status == 1)
							printf("[%d]\tRunning\t\t%s\n",i,job_list[i].command);
						else if(job_list[i].status == 2)
							printf("[%d]\tDone\t\t%s\n",i,job_list[i].command);
					}
				}
			}
		}else if(strcmp(argv[0] , "fg") == 0){
			if(argc == 2){
				int i = atoi(argv[1]);
				if(job_list[i].status == 1){
					//printf("try fg\n");
					signal(SIGTTOU , SIG_IGN);
					int ret = tcsetpgrp(STDIN_FILENO,job_list[i].pid);
					kill(job_list[i].pid , SIGCONT);
					//printf("LOG | %d\n",ret);
					//printf("LOG | current terminal GID %d\n",tcgetpgrp(0));
					waitpid(job_list[i].pid , NULL , 0);
					tcsetpgrp(STDIN_FILENO,shell_gid);
					kill(shell_gid , SIGCONT);
				}
			}else{
				printf("usage : fg [id]\n");
			}
		}else{
			/*
			if((pid = fork()) < 0){ 
				err_sys("fork error\n"); 
			}else if(pid == 0){
				//child
				if (execvp(argv[0],argv) < 0){
					printf("%s: command not found\n",argv[0]);
				}
				exit(0);
			}else{
				//parent
				if(isbg == 0){
					if(waitpid(pid , NULL , 0) < 0){
						{ err_sys("wait error\n"); }
					}
				}
			}
			*/
			runCommand(pid ,argv ,isbg , command_str);
		}
		printf("$ ");
	}
	return 0;
}
