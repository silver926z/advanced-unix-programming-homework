#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <unistd.h> 
#include <errno.h> 
#include <string.h> 
#include <fcntl.h> 

int main(int argc, char **argv){

	int fd1,fd2;

	if(argc > 3)
		fprintf(stderr,"usage: lcp <source> <destination>");

	if((fd1 = open(argv[1], O_RDONLY, 0)) < 0)
        fprintf(stderr,"open fail: %s", argv[1]);

    if((fd2 = open(argv[2], O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR)) < 0)  //Permits the file's owner to read/write it.
        fprintf(stderr,"fail open: %s", argv[2]); 

    struct stat st;
    off_t current_point=0;

    if(fstat(fd1,&st)<0)
    	fprintf(stderr,"fstat wrong\n");

    off_t file_size = st.st_size;

    //printf("%jd\n", (intmax_t)x);
    int x;
    char buff;
    int hole_cnt = 0 ;
    while(current_point < file_size){
    	x = read(fd1, &buff, 1);
    	if(x < 0){
    		fprintf(stderr,"read file wrong\n");
    	}else if (buff != 0 || x==EOF){
            /*if(hole_cnt){
                lseek(fd2,hole_cnt,SEEK_CUR);
                hole_cnt = 0;
            }*/
    		write(fd2, &buff, 1);
    		current_point += 1;
    	}else{
    		write(fd2,"\0",1);
            //hole_cnt++;
    		current_point ++;
    	}
    }


	return 0;
}