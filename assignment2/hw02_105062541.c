#include "hw2.h"
#include <unistd.h>

#define  un_use -2

int mydup2(int oldfd, int newfd){
    //Your implementation here
	int i;
    if(oldfd < 0 || newfd < 0)
    	return -1;
    if(oldfd == newfd)
    	return newfd;
    if(newfd > 1023)  //over maximum file descriptor
    	return -1;
    close(newfd);

    int target = un_use , list[newfd+1];

    for(i = 0;i<=newfd;i++)
    	list[i] = un_use;

    for(i = 0; i <=newfd;i++ ){
    	list[i] = dup(oldfd);
    	if(list[i] == newfd){
    		target = i;
    		break;
    	}else if( list[i] == -1){
    		break;
    	}
    }


    if(target == un_use){  //something happen
    	for(i=0;i<=newfd;i++){
    		if( list[i] == un_use || list[i] == -1)
    			break;
    		close(list[i]);
    	}
        return -1;
    }else{
    	for(i=0; i<target ; i++){
    		close(i);
    	}
    }
    return newfd;
}
