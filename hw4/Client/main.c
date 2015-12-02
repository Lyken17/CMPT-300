//
//  main.c
//  CMPT-300
//
//  Created by Lyken Syu on 9/11/15.
//  Copyright (c) 2015 Lyken Syu. All rights reserved.


#include "awesome.h"
#include "mypipe.h"

extern const char data[];
extern const char data2[];
extern const char owari[];
extern const char finish[];
extern const char filled[];

int main(int argc, const char * argv[]) {
    char curTime[30];

    FILE *fp = fopen(argv[1],"r");
    if (fp == NULL) {
        getCurrentTime(curTime);
        printf("[%s] Process ID #%d did not terminate successfully.\n", curTime, getpid());
        exit(-1);
    }

    fp = fopen(argv[1],"r");
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
                sprintf(msg, "[%s] Process ID #%d decrypted %s successfully.\n", curTime, getpid(), inputFile);
            }
            else if (status == -1)
            {
                //printf("[%s] Process ID #%d cannot find %s.\n", curTime, getpid(), inputFile);
                sprintf(msg, "[%s] Process ID #%d cannot find %s.\n", curTime, getpid(), inputFile);
            }

            1 ? sleep(rand() % 2 + 1) : 0;
            write(ppline[i].toParent[1], msg, strlen(msg));
            //printf("%s\n", msg);
        }

        exit(EXIT_SUCCESS);
    }
    else //Parent process
    {
        char msg[BUFSIZ + 1];
        int freeChild = -1;
        //Read line from file until EOF
        while ((read_line = getline(&line, &len, fp)) != -1) {

            while((freeChild = checkFreeChild(ppline, total, msg)) == -1);
            printf("%s\n", msg);
            //read(ppline[freeChild].toParent[0], buffer, BUFSIZ);

            sscanf(line,"%s%s",inputFile,outputFile);
            //main process
            getCurrentTime(curTime);
            printf("[%s] Child process ID #%d will decrypt %s.\n", curTime, pidList[freeChild], inputFile);
            strcpy(file, line);
            deliverTask(ppline[freeChild], file);
        }

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
                    read(ppline[i].toParent[0], buffer, BUFSIZ);
                    sprintf(msg,"%s",buffer);
                    printf("%s", msg);

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
