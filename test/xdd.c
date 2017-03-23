
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
 
int main() {
   char *str[5], *temp;
   int i, j, n;
 
   printf("\nHow many names do you want to have?");
   scanf("%d", &n);
 
   for (i = 0; i < n; i++) {
      printf("\nEnter the name %d: ", i);
      //flushall();
      gets(str[i]);
   }
 
   for (i = 0; i < n; i++) {
      for (j = 0; j < n - 1; j++) {
         if (strcmp(str[j], str[j + 1]) > 0) {
            strcpy(temp, str[j]);
            strcpy(str[j], str[j + 1]);
            strcpy(str[j + 1], temp);
         }
      }
   }
 
   //flushall();
 
   printf("\nSorted List : ");
   for (i = 0; i < n; i++)
      puts(str[i]);
 
   return (0);
}