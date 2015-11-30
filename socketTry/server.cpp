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

#include "ipaddr.h"

struct params {
        pthread_mutex_t mutex;
        pthread_cond_t done;
        int line;
        FILE *fp;
};

typedef struct params params_t;

void *connection_handler(void *socket_desc)
{
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
    int read_size;
    char *message , client_message[1024], send_message[1024];
    

    sprintf(send_message, "This is message from Server\n");
    //Receive a message from client
    while( (read_size = recv(sock , client_message , 1024 , 0)) > 0 )
    {
        //wait for lock
        pthread_cond_wait (&params.done, &params.mutex);
        //lock these variables
        pthread_mutex_lock(&(*(params_t*)(arg)).mutex);
        printf("Message from client %s\n", client_message);
        //Send the message back to client
        send(sock , send_message , strlen(send_message), MSG_CONFIRM);
        /* Unlock and signal completion.  */
        pthread_mutex_unlock(&(*(params_t*)(arg)).mutex);
        pthread_cond_signal (&(*(params_t*)(arg)).done);

    }

    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }

    //Free the socket pointer
    //free(socket_desc);
    printf("conection closed\n");
    return 0;
}


int main(int argc, char *argv[])
{
    //socket variables
    int listenfd = 0, client_sock = 0;
    struct sockaddr_in serv_addr;
    int port = 1234;
    FILE *fp = fopen("config.txt","r");
    fscanf(fp, "%d", &port);
    fclose(fp);

    char sendBuff[1025];
    time_t ticks;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(port);

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    listen(listenfd, 10);

    char ip[30];
    ipaddr(ip);
    ticks = time(NULL);
    printf("[%.24s] lyrebird.server: PID %d on host %s, port %d\n", ctime(&ticks), getpid(), ip, port);

    while(1)
    {
        struct sockaddr_in their_addr;
        socklen_t addr_size;
        client_sock = accept(listenfd, (struct sockaddr*)&their_addr, &addr_size);

        //get client ip address

        // char *addr = inet_ntoa(their_addr.sin_addr);
        // printf("%s\n", addr);

        pthread_t sniffer_thread;

        if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) &client_sock) < 0)
        {
            perror("could not create thread");
            return 1;
        }

        // int read_size = 0;
        // char client_message[1024];
        // read_size = recv(client_sock, client_message, sizeof(client_message), 0);
        // if (read_size > 0)
        // {
        //     printf("message from client %s\n", client_message);
        // }
        // else
        // {
        //     printf("no message from client\n" );
        // }
        //
        // printf("One connection comes\n");
        // ticks = time(NULL);
        // snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));
        // send(client_sock, sendBuff, strlen(sendBuff), 0);
        //
        //
        // close(client_sock);
     }
}
