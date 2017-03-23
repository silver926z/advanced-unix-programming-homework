#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
int main(){
	int i,xd = open("myfile.txt", O_WRONLY | O_CREAT);
	close(4);
	int xd2 = dup(xd);
	printf("%d %d\n",xd,xd2);
	for(i=0;i<5;i++){
		int tmp = dup(xd);
		printf("%d\n",tmp);
		//close(tmp);	
	}
	close(xd);
	close(xd2);
	return 0;
}
    



