CC=g++
CFLAGS=-c -Wall
TITLE=RTX

all: $(TITLE)

$(TITLE): debug.o initialize.o timingServices.o
	$(CC) debug.o initialize.o timingServices.o

debug.o: debug.cpp rtx.h
	$(CC) $(CFLAGS) debug.cpp

initialize.o: initialize.cpp rtx.h
	$(CC) $(CFLAGS) initialize.cpp

timingServices.o: timingServices.cpp rtx.h
	$(CC) $(CFLAGS) timingServices.cpp

clean:
	rm -rf *.o

run:
	./a.out
