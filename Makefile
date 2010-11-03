CC=g++
CFLAGS=-c -Wall
TITLE=MTE241_RTX

all: $(TITLE)

$(TITLE): debug.o initialize.o RTX.o SignalHandler.o TimingServices.o MsgEnv.o WallClock.o
	$(CC) debug.o initialize.o RTX.o SignalHandler.o TimingServices.o MsgEnv.o WallClock.o

#### EXAMPLE #### 
# <fileName>.o: <dependancies>
#	$(CC) $(CFLAGS) <fileName>.cpp
#
#remember to add to $(TITLE) line!!!

debug.o: debug.cpp debug.h libs.h
	$(CC) $(CFLAGS) debug.cpp

initialize.o: initialize.cpp RTX.h lib/PcbInfo.h
	$(CC) $(CFLAGS) initialize.cpp
	
iprocesses.o: iprocesses.cpp RTX.h SignalHandler.h libs.h
	$(CC) $(CFLAGS) initialize.cpp

RTX.o: RTX.cpp RTX.h libs.h
	$(CC) $(CFLAGS) RTX.cpp

SignalHandler.o: SignalHandler.cpp SignalHandler.h libs.h
	$(CC) $(CFLAGS) SignalHandler.cpp
	
Scheduler.o: Scheduler.cpp lib/PQ.h lib/PCB.h
	$(CC) $(CFLAGS) Scheduler.cpp
	
CCI.o: CCI.cpp CCI.h libs.h
	$(CC) $(CFLAGS) CCI.cpp

TimingServices.o: TimingServices.cpp TimingServices.h
	$(CC) $(CFLAGS) TimingServices.cpp

MsgEnv.o: lib/MsgEnv.cpp lib/MsgEnv.h libs.h
	$(CC) $(CFLAGS) lib/MsgEnv.cpp

WallClock.o: lib/WallClock.cpp lib/WallClock.h libs.h
	$(CC) $(CFLAGS) lib/WallClock.cpp

Queue.o: lib/Queue.cpp lib/Queue.h libs.h
	$(CC) $(CFLAGS) lib/Queue.cpp

PCB.o: lib/PCB.cpp lib/PCB.h
	$(CC) $(CFLAGS) lib/PCB.h
	
Context.o: lib/Context.h lib/Context.cpp
	$(CC) $(CFLAGS) lib/Context.h

clean:
	rm -rf *.o

run:
	./a.out
