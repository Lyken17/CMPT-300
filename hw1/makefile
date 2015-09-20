CC = gcc
CFLAGS  = -DMEMWATCH -DMEMWATCH_STDIO
LIBS = -lm
TARGET = lyrebird
OBJECTS = main.o memwatch.o awesome.o

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
