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

#include "../Share/global.h"

char *Ready = "Ready";


int main(int argc, char *argv[])
{
    int sockfd = 0, n = 0;
    char recvBuff[BUFF_SIZE];
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
    FILE *fp = fopen("../Share/config.txt","r");
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
    char sendMessage[BUFF_SIZE];
    sprintf(sendMessage, "%s", Ready );
    send(sockfd, sendMessage, strlen(sendMessage), MSG_CONFIRM);

    while(1){
        memset(recvBuff, 0, sizeof(recvBuff));
        recv(sockfd, recvBuff, sizeof(recvBuff), 0);

        time_t t;
        srand((unsigned) time(&t));
        sprintf(sendMessage, "%s + %d", hello, rand() % 50 );
        send(sockfd, sendMessage, strlen(sendMessage), MSG_CONFIRM);

        if (strcmp(recvBuff, "GO_HOME_HAVE_FUN") == 0)
        {
            puts("LALALA DOTA gogogo");
            break;
        }

        char inputFile[BUFF_SIZE], outputFile[BUFF_SIZE];
        sscanf(recvBuff, "%s%s",inputFile, outputFile);
        printf("To handle %s, taget is %s\n", inputFile, outputFile);

        sleep(1);
    }

    close(sockfd);

    return 0;
}