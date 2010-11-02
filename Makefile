CC=g++
CFLAGS=-c -Wall
TITLE=MTE241_RTX

all: $(TITLE)

$(TITLE): debug.o initialize.o signalHandler.o timingServices.o Msg_Env.o
	$(CC) debug.o initialize.o signalHandler.o timingServices.o Msg_Env.o

debug.o: debug.cpp rtx.h
	$(CC) $(CFLAGS) debug.cpp

initialize.o: initialize.cpp rtx.h PcbInfo.h
	$(CC) $(CFLAGS) initialize.cpp

signalHandler.o: signalHandler.cpp signalHandler.h
	$(CC) $(CFLAGS) signalHandler.cpp

timingServices.o: timingServices.cpp timingServices.h
	$(CC) $(CFLAGS) timingServices.cpp

Msg_Env.o: Msg_Env.cpp rtx.h
	$(CC) $(CFLAGS) Msg_Env.cpp

clean:
	rm -rf *.o

run:
	./a.out
