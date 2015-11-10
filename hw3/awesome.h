#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>
#include "memwatch.h"
typedef unsigned long long ll;

int value(char a); //trans char to int
char reValue(int n);//trans int to char
ll fastMod(ll base, ll exponent, unsigned long modulus);
char* readString(char *readIn);//pre-handle the string
void handleString(FILE *fp, char *readIn, ll exponent, unsigned long modulus);
//new added
void getCurrentTime(char a[30]);
