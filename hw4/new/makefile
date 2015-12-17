CC = gcc
CFLAGS  = -DMEMWATCH -DMEMWATCH_STDIO -std=c11 -pthread
LIBS = -lm

TARGET_SERVER = lyrebird.server
OBJECTS_SERVER = server.o ipaddr.o memwatch.o

TARGET_CLIENT = lyrebird.client
OBJECTS_CLIENT = main.o awesome.o mypipe.o memwatch.o

.PHONY: all build_server build_client clean

all: build_server build_client

build_client: $(OBJECTS_CLIENT)
	$(CC) $(CFLAGS) -o $(TARGET_CLIENT) $^ $(LIBS)

build_server: $(OBJECTS_SERVER)
	$(CC) $(CFLAGS) -o $(TARGET_SERVER) $^ $(LIBS)

$%.o : $%.c $%.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-rm -f lyrebird.server
	-rm -f lyrebird.client
	-rm -f *.o
	-rm -f *.log
