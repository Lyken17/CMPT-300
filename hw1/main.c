//
//  main.c
//  CMPT-300
//
//  Created by Lyken Syu on 9/11/15.
//  Copyright (c) 2015 Lyken Syu. All rights reserved.
//


#include "awesome.h"

int main(int argc, const char * argv[]) {
    ll exponent = 1921821779;
    ll c[3] = {2385229779,1482371458,3522459006};
    long modulus = 4294434817;

    FILE *fin = fopen(argv[1],"r");
    FILE *fout = fopen(argv[2],"w");
    size_t len = 0;
    ssize_t read;
    char *line = NULL;

    while ((read = getline(&line, &len, fin)) != -1) {
        //printf("Retrieved line of length %zu :\n", read);
        //printf("%s", line);
        //char *readFile = " aa )pyepyt.fd':ymkcnp# dzcs\fkwnze-vy&gfz?h-)!s'\bfia";
        char *handle = readString(line);
        ll* res = handleString(handle,exponent,modulus);
        printf("\n");
        free(handle);
        free(res);
    }

    fclose(fin);
    fclose(fout);
    return 0;
}
