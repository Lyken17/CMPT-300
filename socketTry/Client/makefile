CC = gcc
CFLAGS  = -DMEMWATCH -DMEMWATCH_STDIO -std=c11
LIBS = -lm
TARGET = lyrebird.client
OBJECTS = main.o awesome.o mypipe.o memwatch.o

all: $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $^ $(LIBS)

$%.o : $%.c $%.h
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all clean

clean:
	-rm -f lyrebird
	-rm -f *.o
	-rm -f *.log
	-rm -f *.out
