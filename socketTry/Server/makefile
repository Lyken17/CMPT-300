CC = gcc
CFLAGS  = -DMEMWATCH -DMEMWATCH_STDIO -std=c11 -pthread
LIBS = -lm
TARGET = lyrebird.server
OBJECTS = server.o ipaddr.o memwatch.o

all: $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $^ $(LIBS)

$%.o : $%.c $%.h
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all clean

clean:
	-rm -f tar
	-rm -f *.o
	-rm -f *.log
	-rm -f *.out
