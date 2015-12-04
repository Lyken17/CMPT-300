#ifndef MYPIPE_H
#define MYPIPE_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/select.h>

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

int checkEachChild_2(powerful line, char msg[BUFSIZ+1]);

int checkFreeChild(powerful ppline[], int total, char msg[BUFSIZ+1]);

#endif
