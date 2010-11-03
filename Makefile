CC=g++
CFLAGS=-c -Wall
TITLE=MTE241_RTX

all: $(TITLE)

$(TITLE): debug.o initialize.o signalHandler.o timingServices.o MsgEnv.o WallClock.o
	$(CC) debug.o initialize.o signalHandler.o timingServices.o MsgEnv.o WallClock.o

debug.o: debug.cpp rtx.h
	$(CC) $(CFLAGS) debug.cpp

initialize.o: initialize.cpp rtx.h data_structures/PcbInfo.h
	$(CC) $(CFLAGS) initialize.cpp

signalHandler.o: signalHandler.cpp signalHandler.h
	$(CC) $(CFLAGS) signalHandler.cpp

timingServices.o: timingServices.cpp timingServices.h
	$(CC) $(CFLAGS) timingServices.cpp

MsgEnv.o: MsgEnv.cpp rtx.h
	$(CC) $(CFLAGS) MsgEnv.cpp

WallClock.o: data_structures/WallClock.cpp data_structures/WallClock.h libs.h
	$(CC) $(CFLAGS) data_structures/WallClock.cpp

clean:
	rm -rf *.o

run:
	./a.out
