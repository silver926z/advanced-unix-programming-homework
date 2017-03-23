#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <unistd.h> 
#include <errno.h> 
#include <string.h> 
#include <fcntl.h> 
char buf1[]="ABCDE";
char buf2[]="ZXCVR";
int main(int argc, char **argv){
	int fd;
	if ((fd = open("file10", O_RDWR|O_CREAT, S_IRUSR|S_IWUSR)) < 0)
       printf("creat error");
    if (write(fd, buf1, 5) != 5)
       printf("buf1 write error");
    /* offset now = 10 */
    if (lseek(fd, 17326, SEEK_SET) == -1)
       printf("lseek error");
    /* offset now = 16384 */
    if (write(fd, buf2, 5) != 5)
       printf("buf2 write error");
    /* offset now = 16394 */


	return 0;
}
