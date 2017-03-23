#include <ctype.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#define BUF 32
int main(int argc, char *argv[]){
        FILE* fp,fp2;
                time_t start,end;
        clock_t clock_start,clock_end;
        start = time(NULL);
        clock_start = clock();
        struct timeval CPUstart, CPUend;
        gettimeofday(&CPUstart, NULL);




        /*char buf[5000000];
        memset(buf, 'a', 4999998);
        fp = fmemopen(buf,5000000,"r+");
        fp2 = fmemopen(buf,5000000,"r+");
        int n;
        char buf[BUF];
        while((n=read(fp,buf,BUF))>0)
                if(write(fp2,buf,n)!=n)
                        printf("err");



        end = time(NULL);
        printf("%ld\n",end-start);*/
        int n;
char buf[BUF];
while ((n = read(STDIN_FILENO, buf, BUF)) > 0)
       if (write(STDOUT_FILENO, buf, n) != n)
          printf("");
   if (n < 0)
       printf("");
   end = time(NULL);
        clock_end = clock();
        gettimeofday(&CPUend, NULL);

        
        printf("tick: %f\n",(double)(clock_end - clock_start)/ CLOCKS_PER_SEC);
        printf("wall clock: %f\n",(double) ((CPUend.tv_sec * 1000000 + CPUend.tv_usec)
                  - (CPUstart.tv_sec * 1000000 + CPUstart.tv_usec))/1000000);
        
        printf("CLOCK TIME : %f",((double)(clock_end - clock_start)/ CLOCKS_PER_SEC)+((double) ((CPUend.tv_sec * 1000000 + CPUend.tv_usec)
                  - (CPUstart.tv_sec * 1000000 + CPUstart.tv_usec))/1000000));
	return 0;
}

