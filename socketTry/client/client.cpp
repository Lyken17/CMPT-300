#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <time.h>

int main(int argc, char *argv[])
{
    int sockfd = 0, n = 0;
    char recvBuff[1024];
    struct sockaddr_in serv_addr;

    if(argc != 2)
    {
        printf("\n Usage: %s <ip of server> \n",argv[0]);
        return 1;
    }

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    int port = 1234;
    FILE *fp = fopen("../config.txt","r");
    fscanf(fp, "%d", &port);
    fclose(fp);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    }

    if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
       printf("\n Error : Connect Failed \n");
       return 1;
    }

    char *hello = "Call from Lyken";
    char sendMessage[1024];
    for (size_t i = 0; i < 5; i++) {
        time_t t;
        srand((unsigned) time(&t));
        sprintf(sendMessage, "%s + %d", hello, rand() % 50 );
        send(sockfd, sendMessage, strlen(sendMessage), MSG_CONFIRM);


        memset(recvBuff, 0, sizeof(recvBuff));
        recv(sockfd, recvBuff, sizeof(recvBuff), 0);
        printf("%s", recvBuff);
        sleep(1);
    }


    if(n < 0)
    {
        printf("\n Read error \n");
    }
    close(sockfd);

    return 0;
}
