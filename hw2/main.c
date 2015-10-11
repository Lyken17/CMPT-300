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

    FILE *fp  =fopen(argv[1],"r");
    size_t len = 0;
    ssize_t read;
    char *line = NULL;
    char inputFile[1024], outputFile[1024];
    char fileRecord[20][1024];

    while ((read = getline(&line, &len, fp)) != -1) {
        //printf("%s", line);
        sscanf(line,"%s%s",inputFile,outputFile);
        i++;
        strcpy(fileRecord[i],inputFile);
        pid = fork();
        if (pid == 0 || pid == -1) break;
        //main process
        char a[30];
        getCurrentTime(a);
        printf("[%s] Child process ID #%d created to decrypt %s.\n", a, pid, inputFile);
        // printf("tims is %s\n", a);
        // printf("input is [%s]\n", inputFile);
        // printf("output is [%s]\n", outputFile);
        //oldmain(inputFile, outputFile);
    }

    if (pid == -1)
    {
        printf("alloc new process error!\n");
    }
    else if (pid == 0) //child process
    {
        int status = oldmain(inputFile, outputFile);
        exit(i);
    }
    else //parent process
    {
        while((child = wait(&status)) > 0) {
            //printf("Exit = %d, child ID = %d\n", status/256, child);
            if (status/256 == 0) {
                char a[30];
                getCurrentTime(a);
                printf("[%s] Child process ID #%d did not terminate successfully.\n", a, child);
            }
            else {
                char a[30];
                getCurrentTime(a);
                printf("[%s] Decryption of %s complete. Process ID #%d Exiting.\n", a, fileRecord[status/256], child);
            }
        }
    }
    return 0;
}



int oldmain(char *inputFile, char *outputFile) {
    ll exponent = 1921821779;
    long modulus = 4294434817;
    FILE *fin = fopen(inputFile,"r");
    FILE *fout =
     fopen(outputFile,"w");
    if (fin == NULL) {
        exit(0);
    }

    size_t len = 0;
    ssize_t read;
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
