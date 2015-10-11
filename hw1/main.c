//
//  main.c
//  CMPT-300
//
//  Created by Lyken Syu on 9/11/15.
//  Copyright (c) 2015 Lyken Syu. All rights reserved.



#include "awesome.h"

int main(int argc, const char * argv[]) {
    ll exponent = 1921821779;
    long modulus = 4294434817;

    FILE *fin = fopen(argv[1],"r");
    FILE *fout = fopen(argv[2],"w");
    if (fin == NULL) {
        printf("shit\n");
        exit(-1);
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
