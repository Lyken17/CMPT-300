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
    int i;
    int pid = 0;

    FILE *fp  =fopen(argv[1],"r");
    size_t len = 0;
    ssize_t read;
    char *line = NULL;

    while ((read = getline(&line, &len, fp)) != -1) {
        //printf("%s", line);
        char inputFile[1024], outputFile[1024];
        sscanf(line,"%s%s",inputFile,outputFile);
        pid = fork();
        if (pid == 0 || pid == -1) break;
        // printf("tims is %s\n", a);
        // printf("input is [%s]\n", inputFile);
        // printf("output is [%s]\n", outputFile);
        //oldmain(inputFile, outputFile);
    }
    return 0;
}



int oldmain(char *inputFile, char *outputFile) {
    ll exponent = 1921821779;
    long modulus = 4294434817;

    FILE *fin = fopen(inputFile,"r");
    FILE *fout = fopen(outputFile,"w");
    if (fin == NULL || fout == NULL)
        exit(-1);
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
