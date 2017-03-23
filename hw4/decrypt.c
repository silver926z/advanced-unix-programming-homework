#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <unistd.h> 
#include <errno.h> 
#include <string.h> 
#include <fcntl.h>

char dic[33][10]={
	"password","computer","tigger","summer","internet","service","canada","hello","ranger","shadow","baseball","donald","harley","hockey","letmein","maggie","bare","mustang","snoopy","buster","dragon","nthu","michael","michelle","we","mindy","patrick","bears","andrew","ppap","calvin","changeme","diamond"
};

int main(int argc , char **argv){
	int i,j,k,m,n1,n2,a,b,c;
	FILE *fp;
    char buff[1024];
    char buff2[1024];
    fp = fopen(argv[1], "r");

    fscanf(fp, "%s", buff);
    
   	char *data = strtok(buff,":");
   	char *algo_id;
   	char *salt;
   	char *hash_value;
   	char *data2;
   	for(i=0;i<2 && data != NULL;i++) {
        
        if(i==1){
        	strcpy(buff2,data);
        	data2 = malloc(strlen(data)+1);
        	strcpy(data2,data);
        	data2 = strtok(data2,"$");
        	
       
        	for(j=0;j<4;j++){
        		
        		if(j == 0){
        			algo_id = malloc(strlen(data2)+1);
        			strcpy(algo_id,data2);
        		}
        		else if(j==1){
        			salt = malloc(strlen(data2)+1);
        			strcpy(salt,data2);
        		}
        		else if(j==2){
        			hash_value = malloc(strlen(data2)+1);
        			strcpy(hash_value,data2);
        		}
        		//printf("%s \n %s \n%s\n",algo_id,salt,hash_value);
        		if(j<3)
        			data2 = strtok(NULL, "$");
        	}
        	
        }
        data = strtok(NULL, ":");
    }

    //printf("ID : %s \nSALT:%s \nHASH:%s\n",algo_id,salt,hash_value);
    char candidate[30]="";
    int dicnum = 33;
    char command[100]="$";
    strcat(command,algo_id);
    strcat(command,"$");
    strcat(command,salt);
    strcat(command,"$");
    //printf("[%s]\n",command);
    free(algo_id);
    free(salt);
    free(data2);
    free(data);
    for(a = 0;a<dicnum;a++){
    	strcat(&candidate[0],dic[a]);
    	n1 = strlen(candidate);
    	for(b=0;b<dicnum;b++){
            if(a==b)
                continue;
    		strcat(&candidate[n1],dic[b]);
    		n2 = strlen(candidate);
    		for(c=0;c<dicnum;c++){
                if(b==c)
                    continue;
    			strcat(&candidate[n2],dic[c]);
    			//printf("%s\n",candidate);
    			char *result =  crypt(candidate,command);
    			//printf("%d|%d|%d: \n[@%s] \n [@@%s] \n",a,b,c,buff2,result);
    			if(strcmp(buff2,result)==0){
    				printf("%s\n",candidate);
    				return 0;
    			}
    			candidate[n2] = 0;

    		}
    		candidate[n1] = 0;

    	}
    	candidate[0] = 0;
    }

    free(hash_value);


	return 0;
}
