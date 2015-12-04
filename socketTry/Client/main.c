//
//  main.c
//  CMPT-300
//
//  Created by Lyken Syu on 9/11/15.
//  Copyright (c) 2015 Lyken Syu. All rights reserved.

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
#include "awesome.h"
#include "mypipe.h"

extern const char data[];
extern const char data2[];
extern const char owari[];
extern const char finish[];
extern const char filled[];

char *Ready = "Ready";

int main(int argc, const char * argv[]) {
    char curTime[30];

    if(argc <= 1)
    {
        printf("Please input server ip address\n");
        return -1;
    }
    else if (argc <= 2)
    {
        printf("Please input server port\n" );
        return -1;
    }

    size_t len = 0;
    size_t read_line;
    char *line = NULL;
    char inputFile[1024], outputFile[1024];

    //variables about fork()
    int i = 0;
    pid_t pid = 0;
    pid_t child, parent;

    //Some variables will be used later
    int total = sysconf(_SC_NPROCESSORS_ONLN) - 1;//Cores of computer

    powerful ppline[total];

    int data_processed = 0;

    char buffer[BUFSIZ + 1];
    memset(buffer, '\0', sizeof(buffer));

    char file[2048];
    int pidList[total];
    //Create child
    for (i = 0; i < total; ++i) {

        // pid = 1;
        // break;

        if (pipe (ppline[i].toChild) || pipe (ppline[i].toParent))
        {
            fprintf (stderr, "Pipe failed.\n");
            return EXIT_FAILURE;
        }
        pid = fork();
        if (pid == 0 || pid == -1) break;
        pidList[i] = pid;
    }

    if(pid == -1)
    {
        fprintf(stderr, "Fork failure");
        exit(EXIT_FAILURE);
    }
    else if(pid == 0) //Child process
    {
        int job;
        DEBUG_MODE ? printf("pid = %d, i = %d\n",getpid(), i) : 0;

        write(ppline[i].toParent[1], "\n", strlen("\n"));
        while(1)
        {
            job = getTask(ppline[i], file);
            if (job == -1)
            {
                //received finish signal, exit
                close(ppline[i].toParent[0]);
                close(ppline[i].toParent[1]);
                break;
            }
            //Task part
            sscanf(file,"%s%s",inputFile,outputFile);
            int status = lyrebird(inputFile, outputFile);
            getCurrentTime(curTime);
            char msg[BUFSIZ + 1];
            if (status == 0)
            {
                //printf("[%s] Process ID #%d decrypted %s successfully.\n", curTime, getpid(), inputFile);
                sprintf(msg, "has successfully decrypted %s in process %d.\n", inputFile, getpid());
            }
            else if (status == -1)
            {
                //printf("[%s] Process ID #%d cannot find %s.\n", curTime, getpid(), inputFile);
                sprintf(msg, "has encountered an error: Unable to open file %s in process %d.\n", inputFile, getpid());
            }
            DEBUG_MODE ? sleep(rand() % 2 + 1) : 0;
            write(ppline[i].toParent[1], msg, strlen(msg));
            //printf("%s\n", msg);
        }

        exit(EXIT_SUCCESS);
    }
    else //Parent process
    {
        //new added socket part
        int sockfd = 0;
        char recvBuff[BUFF_SIZE];
        struct sockaddr_in serv_addr;
        int flag = 1;


        char serv_ip[30];
        strcpy(serv_ip, argv[1]);
        //strcpy(serv_ip, "207.23.172.193");

        if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            printf("\n Error : Could not create socket \n");
            for( i = 0; i < total; i++)
            {
                kill(pidList[i], SIGTERM);
            }
            return -1;
        }

        memset(&serv_addr, '0', sizeof(serv_addr));

        int port = atoi(argv[2]);
        //int port = atoi("2333");


        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(port);

        if(inet_pton(AF_INET, serv_ip, &serv_addr.sin_addr)<=0)
        {
            printf("\n inet_pton error occured\n");
            for( i = 0; i < total; i++)
            {
                kill(pidList[i], SIGTERM);
            }
            return -1;
        }

        if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        {
           printf("\n Error : Connect Failed \n");
           for( i = 0; i < total; i++)
           {
               kill(pidList[i], SIGTERM);
           }
           return -1;
        }

        time_t ticks;
        ticks = time(NULL);
        printf("[%.24s] lyrebird client: PID %d connected to server %s on port %d.\n", ctime(&ticks), getpid(),serv_ip, port);

        char sendMessage[BUFF_SIZE];
        send(sockfd, "\n", strlen("\n"), MSG_CONFIRM);

        //pipeline and job handle
        char msg[BUFSIZ + 1];
        int freeChild = -1;

        while(1){
            memset(recvBuff, 0, sizeof(recvBuff));
            recv(sockfd, recvBuff, sizeof(recvBuff), 0);

            if (strcmp(recvBuff, "GO_HOME_HAVE_FUN") == 0)
            {
                //puts("LALALA DOTA gogogo");
                break;
            }

            char inputFile[BUFF_SIZE], outputFile[BUFF_SIZE];
            sscanf(recvBuff, "%s%s",inputFile, outputFile);
            //printf("To handle %s, taget is %s\n", inputFile, outputFile);

            while((freeChild = checkFreeChild(ppline, total, msg)) == -1);
            //printf("%s", msg);
            char tmp[BUFF_SIZE+1];
            //main process
            getCurrentTime(curTime);
            sprintf(tmp,"[%s] Child process ID #%d will decrypt %s.\n", curTime, pidList[freeChild], inputFile);
            strcpy(file, recvBuff);
            deliverTask(ppline[freeChild], file);

            time_t t;
            srand((unsigned) time(&t));
            //sprintf(sendMessage, "%s + %d", "Receive", rand() % 50 );
            memset(sendMessage, 0, sizeof(sendMessage));
            sprintf(sendMessage," %s",msg);
            send(sockfd, sendMessage, strlen(sendMessage), MSG_CONFIRM);
        }

        //Read line from file until EOF
        /*
        while ((read_line = getline(&line, &len, fp)) != -1) {
            //while((freeChild = checkFreeChild(ppline, total, msg)) == -1);
            printf("%s", msg);
            //read(ppline[freeChild].toParent[0], buffer, BUFSIZ);
            sscanf(line,"%s%s",inputFile,outputFile);
            //main process
            getCurrentTime(curTime);
            printf("[%s] Child process ID #%d will decrypt %s.\n", curTime, pidList[freeChild], inputFile);
            strcpy(file, line);
            //deliverTask(ppline[freeChild], file);
        }
        */

        strcpy(file, owari);

        int allChild[total];
        int n = total;
        memset(allChild, 0, sizeof(allChild));

        //Whenever a child finish its task, send him a END_OF_TASK signal.
        while (n > 0)
        {
            for (int i = 0; i < total; ++i)
            {
                if (allChild[i] == 1)
                    continue;
                int temp;
                temp = checkEachChild(ppline[i]);
                if (temp == 0)
                {
                    memset(buffer, 0, sizeof(buffer));
                    read(ppline[i].toParent[0], buffer, BUFSIZ);
                    //printf("%s", buffer);
                    if (strlen(buffer) >= 3) {
                        sprintf(msg,"%s",buffer);
                        //printf("%s", msg);

                        char tmp[BUFF_SIZE+1];
                        sprintf(sendMessage,"%s",msg);
                        recv(sockfd, recvBuff, sizeof(recvBuff), 1);
                        send(sockfd, sendMessage, strlen(sendMessage), MSG_CONFIRM);
                    }
                    allChild[i] = 1;
                    deliverTask(ppline[i], file);
                    n--;
                    close(ppline[i].toChild[0]);
                    close(ppline[i].toChild[1]);
                    int val;
                    waitpid(pidList[i], &val,WUNTRACED);
                    if (val == 0)
                    {
                        //normal exit
                        DEBUG_MODE ? printf("PID:%d goes home and is playing dota!\n", pidList[i]) : 0;
                    }
                    else
                    {
                        //abnormal exit
                        printf("[%s] Process ID #%d did not terminate successfully.\n", curTime, pidList[i]);
                        close(ppline[i].toParent[0]);
                        close(ppline[i].toParent[1]);
                    }
                }
            }
        }
        sprintf(sendMessage,"%s","FINISH_DONE");
        send(sockfd, sendMessage, strlen(sendMessage), MSG_CONFIRM);
        printf("[%.24s] lyrebird client: PID %d completed its tasks and is exiting successfully.\n", ctime(&ticks), getpid());
        close(sockfd);
        exit(EXIT_SUCCESS);
    }
    return 0;
}



int lyrebird(char *inputFile, char *outputFile) {
    ll exponent = 1921821779;
    long modulus = 4294434817;

    FILE *fin = fopen(inputFile,"r");
    if (fin == NULL) {
        return -1;
    }

    FILE *fout = fopen(outputFile,"w");

    size_t len = 0;
    size_t read;
    char *line = NULL;

    while ((read = getline(&line, &len, fin)) != -1) {
        char *handle = readString(line);
        handleString(fout, handle,exponent,modulus);
        free(handle);
    }

    fclose(fin);
    fclose(fout);

    return 0;
}
