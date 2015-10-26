//
//  main.c
//  CMPT-300
//
//  Created by Lyken Syu on 9/11/15.
//  Copyright (c) 2015 Lyken Syu. All rights reserved.


#include "awesome.h"

int main(int argc, const char * argv[]) {
    pid_t child, parent;
    int status=0;
    int i = 0;
    int pid = 0;
    char curTime[30];

    FILE *fp = fopen(argv[1],"r");
    if (fp == NULL) {
        getCurrentTime(curTime);
        printf("[%s] Process ID #%d did not terminate successfully.\n", curTime, getpid());
        exit(-1);
    }
    size_t len = 0;
    size_t read;
    char *line = NULL;
    char inputFile[1024], outputFile[1024];


    while ((read = getline(&line, &len, fp)) != -1) {
        //printf("%s", line);
        sscanf(line,"%s%s",inputFile,outputFile);
        i++;
        pid = fork();
        if (pid == 0 || pid == -1) break;
        //main process
        getCurrentTime(curTime);
        printf("[%s] Child process ID #%d created to decrypt %s.\n", curTime, pid, inputFile);
        // printf("tims is %s\n", a);
        // printf("input is [%s]\n", inputFile);
        // printf("output is [%s]\n", outputFile);
        //oldmain(inputFile, outputFile);
    }

    if (pid == -1)
    {
        getCurrentTime(curTime);
        printf("[%s] Process ID #%d did not terminate successfully.\n", curTime, getpid());
        exit(-1);
    }
    else if (pid == 0) //child process
    {
        int status = lyrebird(inputFile, outputFile);
        getCurrentTime(curTime);
        printf("[%s] Decryption of %s complete. Process ID #%d Exiting.\n", curTime, inputFile, getpid());
        exit(0);
    }
    else //parent process
    {
        while((child = wait(&status)) > 0) {
            if (status/256 != 0) {
                getCurrentTime(curTime);
                printf("[%s] Child process ID #%d did not terminate successfully.\n", curTime, child);
            }
        }
    }
    return 0;
}



int lyrebird(char *inputFile, char *outputFile) {
    ll exponent = 1921821779;
    long modulus = 4294434817;
    FILE *fin = fopen(inputFile,"r");
    FILE *fout = fopen(outputFile,"w");

    if (fin == NULL) {
        exit(-1);
    }

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
