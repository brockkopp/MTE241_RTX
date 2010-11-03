CC=g++
CFLAGS=-c -Wall
TITLE=MTE241_RTX

all: $(TITLE)

$(TITLE): debug.o initialize.o SignalHandler.o TimingServices.o MsgEnv.o WallClock.o
	$(CC) debug.o initialize.o SignalHandler.o TimingServices.o MsgEnv.o WallClock.o

#### EXAMPLE #### 
# <fileName>.o: <dependancies>
#	$(CC) $(CFLAGS) <fileName>.cpp
#
#remember to add to $(TITLE) line!!!

debug.o: debug.cpp rtx.h
	$(CC) $(CFLAGS) debug.cpp

initialize.o: initialize.cpp rtx.h data_structures/PcbInfo.h
	$(CC) $(CFLAGS) initialize.cpp

SignalHandler.o: SignalHandler.cpp SignalHandler.h libs.h
	$(CC) $(CFLAGS) SignalHandler.cpp

timingServices.o: TimingServices.cpp TimingServices.h
	$(CC) $(CFLAGS) TimingServices.cpp

MsgEnv.o: data_structures/MsgEnv.cpp data_structures/MsgEnv.h libs.h
	$(CC) $(CFLAGS) data_structures/MsgEnv.cpp

WallClock.o: data_structures/WallClock.cpp data_structures/WallClock.h libs.h
	$(CC) $(CFLAGS) data_structures/WallClock.cpp

clean:
	rm -rf *.o

run:
	./a.out
