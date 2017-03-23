#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <linux/unistd.h>     
#include <linux/kernel.h>    



int main(int argc, char **argv){
    int sockfd, newsockfd, port, clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int  n;
    if (argc < 2) {
        fprintf(stderr,"usage %s port\n", argv[0]);
        exit(0);
    }

    /* First call to socket() function */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    port = atoi(argv[1]);

    int sock_opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (void*)&sock_opt, sizeof(sock_opt) ) == -1){
        perror("ERROR!!! while reuse addr\n");
        return -1;
    }
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    /* Now bind the host address using bind() call.*/
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR on binding");
        exit(1);
    }
        
    /* Now start listening for the clients, here process will
        * go in sleep mode and will wait for the incoming connection
    */

    listen(sockfd,10);
    clilen = sizeof(cli_addr);
    pid_t pid;
    while(1){
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        if ((pid = fork()) < 0) fprintf(stderr,"fork error");
        if(pid == 0){
            if (newsockfd < 0) {
                perror("ERROR on accept");
                exit(1);
            }
            
            struct sysinfo data;
            
            while(1){
                sysinfo(&data);
                bzero(buffer,256);
                
                char text[50];
                bzero(text,50);
                sprintf(text,"current procs: %d\n",data.procs);
                
                n = write(newsockfd,text,50);

                if (n < 0) {
                    perror("ERROR writing to socket");
                    exit(1);
                }
                sleep(1);
            }
            close(newsockfd);
        }

        close(newsockfd);
    }
        
    return 0;
}