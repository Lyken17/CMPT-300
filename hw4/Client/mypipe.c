#include "mypipe.h"


const char data[] = "Hello pipe!";
const char owari[] = "__END__OF__TASK__";
const char finish[] = "__WORK__DONE__";
const char filled[] = "\0";
const char success[] = "__SUCCESS__DOEN__";

int data_processed = 0;
char buffer[BUFSIZ + 1];

int scheduleMethod(char *line)
{
	int plan = 0;
	if (strcmp(line, "round robin\n") == 0)
    {
        plan = ROUND_ROBIN;
        //printf("This is round robin scheduling\n");
    }
    else if (strcmp(line, "fcfs\n") == 0)
    {
        plan = FCFS;
        //printf("This is FCFS scheduling\n");
    }
    else
    {
        //printf("Not legal option!\n");
        exit(EXIT_FAILURE);
    }
	return plan;
}

int getTask(powerful line, char file[2048])
{
	data_processed = read(line.toChild[0], buffer, BUFSIZ);
	DEBUG_MODE ? printf("PID:%d Read %d bytes: %s\n",getpid(), data_processed, buffer) : 0;
	strcpy(file, buffer);

	char *pos = strstr(buffer, owari);
	if (pos)
		return -1;

	return 0;
}

int workDone(powerful line)
{
	data_processed = write(line.toParent[1], finish, strlen(finish));

	return 0;
}

int deliverTask(powerful line, char file[2048])
{
	data_processed = write(line.toChild[1], file, strlen(file));

	DEBUG_MODE ? printf("PID:%d Wrote %d bytes: %s\n",getpid(), data_processed, file) : 0;
	return 0;
}

int checkEachChild(powerful line)
{
	int retval;

	fd_set set;
	struct timeval timeout;
	timeout.tv_sec = 0;
  	timeout.tv_usec = 50000;

  	FD_ZERO (&set);
	FD_SET (line.toParent[0], &set);

    //listen to pipeline's status
	retval = select (FD_SETSIZE, &set, NULL, NULL, &timeout);
	if (retval == -1)
        perror("select()");
    else if (retval)
    {
    	//Data is avaliable

        return 0;
    }
    else
    {
    	//There is no data in pipeline.
    	return -1;
    }
	return -1;
}

int checkEachChild_2(powerful line, char msg[BUFSIZ+1])
{
	int available = checkEachChild(line);
	if (available == 0)
	{
		read(line.toParent[0], buffer, BUFSIZ);
		sprintf(msg,"%s",buffer);
	}
	return available;
}

int checkFreeChild(powerful ppline[], int total, char msg[BUFSIZ+1])
{
	fd_set set;
	/* Initialize the file descriptor set. */
	int retval;
	struct timeval timeout;
	timeout.tv_sec = 0;
  	timeout.tv_usec = 50000;

  	for (int i = 0; i < total; ++i)
  	{
  		int available;
  		available = checkEachChild(ppline[i]);
  		if (available == 0)
  		{
  			read(ppline[i].toParent[0], buffer, BUFSIZ);
			if (strlen(buffer) >= 2)
				sprintf(msg,"%s",buffer);
			else
				msg[0] = '\0';
  			//printf("-----------Data is available now.  ");
	        //printf("The free child is %d-----------\n", i);
  			return i;
  		}
  	}
  	//printf("No data within one round.\n");
  	return -1;

}
