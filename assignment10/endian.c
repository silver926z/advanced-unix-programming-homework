#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>


int main(int argc, char **argv){
   uint32_t   val  = 0x12345678;
   char     * buff = (char *)&val;
   printf("%d\n",buff[0]);
   if (buff[0] == 0x12)
   {
      printf("Big endian\n");
   } else if(buff[0]==0x78){
      printf("Little endian\n");
   };
   return 0;
}