CC=g++
CFLAGS= -c -Wall
TITLE=MTE241_RTX

all: $(TITLE)

$(TITLE):     debug.o initialize.o iprocesses.o RTX.o SignalHandler.o Scheduler.o CCI.o TimingServices.o MsgEnv.o WallClock.o Queue.o PCB.o Context.o
	@$(CC) debug.o initialize.o iprocesses.o RTX.o SignalHandler.o Scheduler.o CCI.o TimingServices.o MsgEnv.o WallClock.o Queue.o PCB.o Context.o

#### EXAMPLE #### 
# <fileName>.o: <dependancies>
#	@$(CC) $(CFLAGS) <fileName>.cpp			#the '@' hides the command from the user (output is still displayed)
#
#remember to add to $(TITLE) line!!!

debug.o: debug.cpp debug.h libs.h
	@$(CC) $(CFLAGS) debug.cpp

initialize.o: initialize.cpp RTX.h lib/PcbInfo.h
	@$(CC) $(CFLAGS) initialize.cpp
	
iprocesses.o: iprocesses.cpp RTX.h SignalHandler.h libs.h
	@$(CC) $(CFLAGS) iprocesses.cpp

RTX.o: RTX.cpp RTX.h lib/PcbInfo.h lib/PCB.h 
	@$(CC) $(CFLAGS) RTX.cpp

SignalHandler.o: SignalHandler.cpp SignalHandler.h libs.h
	@$(CC) $(CFLAGS) SignalHandler.cpp
	
Scheduler.o: Scheduler.cpp Scheduler.h lib/PQ.h lib/PCB.h
	@$(CC) $(CFLAGS) Scheduler.cpp
	
CCI.o: CCI.cpp CCI.h libs.h
	@$(CC) $(CFLAGS) CCI.cpp

TimingServices.o: TimingServices.cpp TimingServices.h
	@$(CC) $(CFLAGS) TimingServices.cpp

MsgEnv.o: lib/MsgEnv.cpp lib/MsgEnv.h libs.h
	@$(CC) $(CFLAGS) lib/MsgEnv.cpp

WallClock.o: lib/WallClock.cpp lib/WallClock.h libs.h
	@$(CC) $(CFLAGS) lib/WallClock.cpp
	
MsgTrace.o: MsgTrace.cpp MsgTrace.h libs.h
	$(CC) $(CFLAGS) MsgTrace.cpp

Queue.o: lib/Queue.cpp lib/Queue.h libs.h
	@$(CC) $(CFLAGS) lib/Queue.cpp

PCB.o: lib/PCB.cpp lib/PCB.h lib/PcbInfo.h
	@$(CC) $(CFLAGS) lib/PCB.cpp
	
Context.o: lib/Context.cpp lib/Context.h
	@$(CC) $(CFLAGS) lib/Context.cpp

clean:
	@rm -rf *.o

run:
	@./a.out
