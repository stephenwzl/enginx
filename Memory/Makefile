TARGET = mem.o
CC=gcc
CFLAGS = -c -g -DDEBUG -Wall -ansi -pedantic
OBJS = memory.o storage.o

INCLUDES = \
	-I../include

$(TARGET):$(OBJS)
	ld -r -o $@ $(OBJS)
testp : $(OBJS) main.o
	$(CC) -o $@ $(OBJS) main.o
.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) $*.c
memory.o: memory.c memory.h ../include/Mem.h
storage.o: storage.c memory.h ../include/Mem.h
