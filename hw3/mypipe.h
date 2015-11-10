#ifndef MYPIPE_H
#define MYPIPE_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DEBUG_MODE 1

struct powerful
{
	int toChild[2];
	int toParent[2];
};

typedef struct powerful powerful;

int getTask(powerful line, char file[1024]);

int workDone(powerful line);

int deliverTask(powerful line, char file[1024]);

int checkEachChild(powerful line);

int checkFreeChild(powerful ppline[], int total);

#endif
