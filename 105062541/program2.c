#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main(){
    time_t t = time(0);
    struct tm tm1, tm2;
    char buf[256];
    gmtime_r(&t, &tm1);
    localtime_r(&t, &tm2);

   // printf(" localtime -> asctime: %s", asctime(&tm2));
   // strftime(buf, sizeof(buf), "%c %Z (%z)", &tm1);
    //printf("   gmtime -> strftime: %s\n", buf);
    strftime(buf, sizeof(buf), "%X, %A %B%e, %G", &tm2);
   // printf("localtime -> strftime: %s\n", buf);
    printf("%s\n",buf);
  return 0;
}
