//
//  main.c
//  CMPT-300
//
//  Created by Lyken Syu on 9/11/15.
//  Copyright (c) 2015 Lyken Syu. All rights reserved.


#include "awesome.h"
#include "mypipe.h"

int main(int argc, const char * argv[]) {
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

    //Read the scheduling option
    read = getline(&line, &len, fp);
    printf("%s", line);
    int plan = 0;
    if (strcmp(line, "round robin\n") == 0)
    {
        plan = 1;
        printf("This is round robin scheduling\n");
    }
    else if (strcmp(line, "fcfs\n") == 0)
    {
        plan = 2;
        printf("This is FCFS scheduling\n");
    }
    else
    {
        printf("Not legal option!\n");
        exit(EXIT_FAILURE);
    }
    //variables about fork()
    int i = 0;
    pid_t pid = 0;
    pid_t child, parent;

    //Some variables will be used later
    int total = sysconf(_SC_NPROCESSORS_ONLN) - 1;//Cores of computer
    total = 3;
	powerful ppline[total];

    int data_processed = 0;
	const char data[] = "Hello pipe!";
	const char data2[] = "Hello doubi!";
	const char owari[] = "__END__OF__TASK__";
	const char finish[] = "__WORK__DONE__";
	const char filled[] = "\0";
	char buffer[BUFSIZ + 1];
	memset(buffer, '\0', sizeof(buffer));

    char file[2048];

    //Create child
    for (i = 0; i < total; ++i) {
		if (pipe (ppline[i].toChild) || pipe (ppline[i].toParent))
		{
			fprintf (stderr, "Pipe failed.\n");
			return EXIT_FAILURE;
		}

		pid = fork();
		if (pid == 0 || pid == -1) break;
	}

    if(pid == -1)
	{
		fprintf(stderr, "Fork failure");
		exit(EXIT_FAILURE);
	}
    else if(pid == 0)
	{
        //Child process
		int job;
		 1 ? printf("pid = %d, i = %d\n",getpid(), i) : 0;

		workDone(ppline[i]);
		while(1)
		{

			job = getTask(ppline[i], file);
			if (job == -1)
			{
				DEBUG_MODE ? printf("PID:%d goes home and is playing dota!\n", getpid()) : 0;
				break;
			}
			//Task part
			sscanf(file,"%s%s",inputFile,outputFile);
            int status = lyrebird(inputFile, outputFile);
            getCurrentTime(curTime);
            if (status == 0)
            {
                printf("[%s] Decryption of %s complete. Process ID #%d Exiting.\n", curTime, inputFile, getpid());
            }
            else
            {
                printf("[%s] Process ID #%d did not terminate successfully.\n", curTime, getpid());
            }
            sleep(rand() % 3);
            //
			workDone(ppline[i]);
		}

		exit(EXIT_SUCCESS);
	}
    else
	{
		int freeChild = -1;
        while ((read = getline(&line, &len, fp)) != -1) {
            //printf("%s", line);
            while((freeChild = checkFreeChild(ppline, total)) == -1);

            sscanf(line,"%s%s",inputFile,outputFile);
            //main process
            getCurrentTime(curTime);
            printf("[%s] Child process ID #%d created to decrypt %s.\n", curTime, pid, inputFile);
            strcpy(file, line);
            deliverTask(ppline[freeChild], file);
        }

		strcpy(file, owari);

		int allChild[total];
		int n = total;
		memset(allChild, 0, sizeof(allChild));
		while (n > 0)
		{
			for (int i = 0; i < total; ++i)
			{
				if (allChild[i] == 1)
					continue;
				int temp;
				temp = checkEachChild(ppline[i]);
				if (temp == 0)
				{
					allChild[i] = 1;
					deliverTask(ppline[i], file);
					n--;
				}
			}
		}

		pid_t child;
		int status;

		exit(EXIT_SUCCESS);
	}
    return 0;
}



int lyrebird(char *inputFile, char *outputFile) {
    ll exponent = 1921821779;
    long modulus = 4294434817;
    FILE *fin = fopen(inputFile,"r");
    FILE *fout = fopen(outputFile,"w");
    if (fin == NULL) {
        return -1;
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
