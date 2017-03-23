#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
int wcnt,icnt,ccnt,wnum;
int isw , isc , isi ,iscmd,cmdnum;
int main(int argc, char *argv[]){
	char str[1000][257],temp[257];
	int i=0,j,num=0;
	wcnt = icnt = ccnt = 0;
    iscmd = isw = isc = isi = cmdnum = 0;

	FILE *fp;
	if(argc < 2){
    	//printf("usage: wc [-clmw] [file ...]\n");
      fprintf(stderr,"usage: wc [-clmw] [file ...]\n");
    	return 1;
    }

    for (i=1;i<argc;i++){
    	if(argv[i][0] == '-'){
    		for(j=1;j<strlen(argv[i]);j++){
    			if(argv[i][j] == 'c'){
    				iscmd = isc = 1;
    				
    			}else if(argv[i][j] == 'i'){
    				iscmd = isi = 1;
    				
    			}else if(argv[i][j] == 'w'){
    				iscmd = isw = 1;
    				if(i==argc-1){
    					printf("uniq: option requires an argument -- 'w'\n");
    					return 1;
    				}
    				else{
    					wnum = atoi(argv[i+1]);
    					//printf("XDD%d\n",wnum);
    				}

    			}else{
    				fprintf(stderr,"uniq: invalid option -- '%c'\nTry 'uniq --help' for more information.\n",argv[i][j]);
    				return 1;
    			}
    				
    		}
    	}else if(isw == 0){
    		fp = fopen(argv[i], "r");
    		if(fp == NULL){
        		fprintf(stderr,"wc: %s: No such file or directory\n",argv[i]);
		        return 1;
		    }
		    int file_num = i;
    	}
    }
	i=0;

	while(fscanf(fp,"%s",str[i])!=EOF){
		//printf("%s\n",str[i]);
		i++;
	}
	fclose(fp);
	num = i;
	//printf("XDD\n");
	for(i=0; i<num; i++) {
      for(j=0; j<num-1; j++) {
      	if(isi == 1){
      		if(isw == 1){
		        if(strncasecmp(str[j], str[j + 1],wnum) > 0) {
		            strcpy(temp, str[j]);
		            strcpy(str[j], str[j + 1]);
		            strcpy(str[j + 1], temp);
		        }
	    	}else{
	    		if(strcasecmp(str[j], str[j + 1]) > 0) {
		            strcpy(temp, str[j]);
		            strcpy(str[j], str[j + 1]);
		            strcpy(str[j + 1], temp);
		        }
	    	}
     	}else if(isi == 0){
     		if(isw == 1){
		        if(strncmp(str[j], str[j + 1],wnum) > 0) {
		            strcpy(temp, str[j]);
		            strcpy(str[j], str[j + 1]);
		            strcpy(str[j + 1], temp);
		        }
	    	}else{
	    		if(strcmp(str[j], str[j + 1]) > 0) {
		            strcpy(temp, str[j]);
		            strcpy(str[j], str[j + 1]);
		            strcpy(str[j + 1], temp);
		        }
	    	}
     	}
      }
   }
   /*for(i=0;i<num;i++)
   	printf("%s\n",str[i]);
   printf("=========================\n");*/
   char buf[257];
   int cur_buff_cnt=0;
   if(isc == 0){//count
   		if(isi == 1){ // caseinsensitive
   			for(i=0;i<num;i++){
   				if(strcasecmp(str[i],buf)!=0 || i == num-1){
   					printf("%s\n",str[i]);
   					strcpy(buf,str[i]);
   				}else if(strcasecmp(str[i],buf)==0){
   					strcpy(buf,str[i]);
   					
   				}
   			}

   		}else{
   			for(i=0;i<num;i++){
   				if(strcmp(str[i],buf)!=0 || i == num-1){
   					printf("%s\n",str[i]);
   					strcpy(buf,str[i]);
   				}else if(strcmp(str[i],buf)==0){
   					strcpy(buf,str[i]);
   					
   				}
   			}
   		}
   }else if(isc == 1){
   		//strcpy(buf,str[0]);
   		if(isi == 1){ // caseinsensitive
   			for(i=0;i<num;i++){
   				if(strcasecmp(str[i],str[i+1])!=0|| i == num-1){
   					printf("      %d %s\n",cur_buff_cnt+1,str[i]);
   					cur_buff_cnt = 0;
   					strcpy(buf,str[i]);
   				}else if(strcasecmp(str[i],str[i+1])==0){
   					strcpy(buf,str[i]);
   					cur_buff_cnt++;
   				}
   			}

   		}else{
   			for(i=0;i<num;i++){
   				if(i == num-1 || strcmp(str[i],str[i+1])!=0){
   					printf("      %d %s\n",cur_buff_cnt+1,str[i]);
   					cur_buff_cnt = 0;
   					strcpy(buf,str[i]);
   				}else if(strcmp(str[i],str[i+1])==0){
   					strcpy(buf,str[i]);
   					cur_buff_cnt++;
   				}
   			}
   		}
   }

	return 0;
}
