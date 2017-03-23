#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>


char input[1000];
char arg[1000];
char *input_parse;
int k=0;
int invisible = 0;

int main(int argc, char **argv){
	char buf[80];
	pid_t pid=getpid();
	//char*ps_argv[]={"ps","a", NULL};
	int a;
	char*ps_argv[100];
	char cmd[100];
	while(1){
		getcwd(buf, sizeof(buf));
    	//printf("%s:  ", buf);
		printf("$");
		fgets(input,1000,stdin);

		
		input_parse = strtok(input , " \n");
		if(input_parse == NULL){
			continue;
		}

		if(strcmp(input_parse, "pwd") == 0){
    		getcwd(buf, sizeof(buf));
    		printf("%s\n", buf);
		}else if (strcmp(input_parse , "cd")==0 ){
			input_parse = strtok(NULL , " \n");
			chdir(input_parse);
		}else {
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
			}else{
				pid = fork();
				//while(wait(NULL)>0); // wait until child finish
				// waitpid(pid,NULL,0);
				if(pid != 0)
					waitpid(pid,NULL,0);
			}
			
			
		}
		

		if(pid == 0){
			int t;
			if (invisible == 1)
				close(0);
			t = execvp(cmd,ps_argv);
			if(t < 0)
				printf("%s: command not found\n",cmd);
			exit(0);
		}
	}
	

	return 0;
}