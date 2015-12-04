#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "../Share/global.h"
#include "ipaddr.h"

struct threadInfo {
    int socket_desc;
    char ip[30];
};

char *Ready = "Ready";
int count = 0;
FILE *fp;
int hasReachEnd = 0;


void *connection_handler(void *information)
{
    threadInfo clientInfo = *(threadInfo *) information;
    //Get the socket descriptor
    //int sock = *(int*)socket_desc;
    int sock = clientInfo.socket_desc;
    int read_size;
    char *message , client_message[BUFF_SIZE], send_message[BUFF_SIZE];
    time_t ticks;
    ticks = time(NULL);

    printf("[%.24s] Successfully connected to lyrebird client %s\n", ctime(&ticks), clientInfo.ip);

    //Receive a message from client
    while( (read_size = recv(sock , client_message , BUFF_SIZE , 0)) > 0 )
    {
        if (strlen(client_message) >= 3)
            printf("%s", client_message);
        memset(client_message, 0, sizeof(client_message));
        //if (strstr(client_message, Ready) != NULL)
        if (1)
        {
            //diliver task
            size_t len = 0;
            size_t read;
            char *line = NULL;
            char inputFile[BUFF_SIZE], outputFile[BUFF_SIZE];

            read = getline(&line, &len, fp);
            if (read == -1)
            {
                sprintf(send_message,"GO_HOME_HAVE_FUN");
                send(sock , send_message , strlen(send_message), MSG_CONFIRM);
                read_size = 0;
                hasReachEnd = 1;
                break;
            }
            sscanf(line,"%s%s",inputFile,outputFile);

            //printf("Deliver %s\n", inputFile);
            sprintf(send_message, "%s\n", line);
            //Send the message back to client
            send(sock , send_message , strlen(send_message), MSG_CONFIRM);
        }
    }
    while( (read_size = recv(sock , client_message , BUFF_SIZE , 0)) > 0 )
    {
        if (strstr(client_message, "FINISH_DONE"))
        {
            ticks = time(NULL);
            printf("[%.24s] lyrebird client %s has disconnected expectedly\n", ctime(&ticks), clientInfo.ip);
            break;
        }
        printf("%s", client_message);
        memset(client_message, 0, sizeof(client_message));
    }

    if(read_size == 0)
    {
        puts("Client disconnected unexpectedly");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }

    //Free the socket pointer
    //free(socket_desc);
    free(information);

    return 0;
}


int main(int argc, char *argv[])
{
    //socket variables
    int listenfd = 0, client_sock = 0;
    struct sockaddr_in serv_addr;

    int port = 2333;

    fp = fopen("config_file.txt", "r");
    if (fp == NULL) {
        printf("Config file doesn't exsit\n");
    }

    char sendBuff[BUFF_SIZE];
    time_t ticks;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listenfd < 0) {
        printf("socket error\n");
        return -1;
    }

    memset(sendBuff, '0', sizeof(sendBuff));
    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    if (bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Bind error\n" );
    }

    socklen_t len = sizeof(serv_addr);
    if (getsockname(listenfd, (struct sockaddr *)&serv_addr, &len) == -1) {
        perror("getsockname");
        return -1;
    }

    listen(listenfd, 10);

    char ip[30];
    ipaddr(ip);
    ticks = time(NULL);
    printf("[%.24s] lyrebird.server: PID %d on host %s, port %d\n", ctime(&ticks), getpid(), ip, ntohs(serv_addr.sin_port));

    while(1)
    {
        struct sockaddr_in their_addr;
        socklen_t addr_size;
        client_sock = accept(listenfd, (struct sockaddr*)&their_addr, &addr_size);
        //get client ip address

        char *addr = inet_ntoa(their_addr.sin_addr);
        //printf("%s\n", addr);

        if (hasReachEnd == 1)
        {
            char send_message[BUFF_SIZE+1];
            sprintf(send_message,"GO_HOME_HAVE_FUN");
            send(client_sock , send_message , strlen(send_message), MSG_CONFIRM);
            break;
        }

        pthread_t sniffer_thread;
        threadInfo *clientInfo = (threadInfo*) malloc(sizeof(threadInfo));
        clientInfo->socket_desc = client_sock;
        strcpy(clientInfo->ip, addr);

        if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) clientInfo) < 0)
        {
            perror("could not create thread");
            return 1;
        }

     }
}
