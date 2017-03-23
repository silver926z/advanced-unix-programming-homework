#include <dirent.h>
#include <limits.h>
#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char *argv[]){
	

	char* folder=argv[1];                                    

    DIR* dir_p;
    struct dirent* dir_element;
    struct stat file_info;

    
    dir_p=opendir(folder);

    
    while(dir_element = readdir(dir_p)){
        lstat(dir_element->d_name, &file_info);         

        if ((file_info.st_mode & S_IFMT)==S_IFLNK){
        	char buf[1024];
			ssize_t len;
			if ((len = readlink(dir_element->d_name, buf, sizeof(buf)-1)) != -1)
    			buf[len] = '\0';
    		printf("%s->%s\n",dir_element->d_name,buf);

         
        }
    }

}