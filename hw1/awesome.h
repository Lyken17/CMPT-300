#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "memwatch.h"
typedef unsigned long long ll;

int value(char a);
char reValue(int n);
ll fastMod(ll base, ll exponent, unsigned long modulus);
char* readString(char *readIn);
void handleString(FILE *fp, char *readIn, ll exponent, unsigned long modulus);
