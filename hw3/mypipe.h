#ifndef MYPIPE_H
#define MYPIPE_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DEBUG_MODE 0
#define ROUND_ROBIN 1
#define FCFS 2

struct powerful
{
	int toChild[2];
	int toParent[2];
};

struct ffname
{
	char name[2048];
};

typedef struct powerful powerful;
typedef struct ffname ffname;

int scheduleMethod(char *line);

int getTask(powerful line, char file[2048]);

int workDone(powerful line);

int deliverTask(powerful line, char file[2048]);

int checkEachChild(powerful line);

int checkFreeChild(powerful ppline[], int total);

#endif
