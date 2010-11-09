CMP=g++
CMPFLGS= -c -Wall	#compile only, show all warnings
LNKFLGS= -o
TITLE=RTX.out
OBJ= debug.o initialize.o iprocesses.o RTX.o SignalHandler.o Scheduler.o CCI.o TimingServices.o MsgEnv.o WallClock.o Queue.o PQ.o PCB.o Context.o userProcesses.o tests.o tools.o
KB=KB.out
KB_OBJ=keyboard.o
CRT=CRT.out
CRT_OBJ=crt.o

##Command List
all: preclean $(KB) $(CRT) $(TITLE) clean run

preclean:
	@rm -f $(TITLE) $(KB) $(CRT) $(OBJ) $(KB_OBJ) $(CRT_OBJ)

clean:
	@rm -f $(OBJ) $(KB_OBJ) $(CRT_OBJ)	#Delete all object files

run:
	@./$(TITLE)				#Run main executable after build

##Linking
$(TITLE):  $(OBJ)					
	@$(CMP) $(LNKFLGS) $(TITLE) $(OBJ)

$(KB): $(KB_OBJ)
	@$(CMP) $(LNKFLGS) $(KB) $(KB_OBJ)

$(CRT): $(CRT_OBJ)
	@$(CMP) $(LNKFLGS) $(CRT) $(CRT_OBJ)

##Compiling

#### EXAMPLE #### 
# <fileName>.o: <dependancies>
#	@$(CMP) $(CMPFLGS) <fileName>.cpp			#the '@' hides the command from the user (output is still displayed)
#
#remember to add to $(OBJ) line!!!
CCI.o: CCI.cpp CCI.h
	@$(CMP) $(CMPFLGS) CCI.cpp

Context.o: lib/Context.cpp lib/Context.h
	@$(CMP) $(CMPFLGS) lib/Context.cpp
	
debug.o: debug.cpp debug.h
	@$(CMP) $(CMPFLGS) debug.cpp

initialize.o: initialize.cpp RTX.h lib/PcbInfo.h
	@$(CMP) $(CMPFLGS) initialize.cpp
	
iprocesses.o: iprocesses.cpp RTX.h SignalHandler.h
	@$(CMP) $(CMPFLGS) iprocesses.cpp

MsgEnv.o: lib/MsgEnv.cpp lib/MsgEnv.h
	@$(CMP) $(CMPFLGS) lib/MsgEnv.cpp
	
MsgServ.o:MsgServ.cpp MsgServ.h
	@$(CMP) $(CMPFLGS) MsgServ.cpp

MsgEnv.o: lib/MsgEnv.cpp lib/MsgEnv.h
	@$(CMP) $(CMPFLGS) lib/MsgEnv.cpp
	
PCB.o: lib/PCB.cpp lib/PCB.h lib/PcbInfo.h
	@$(CMP) $(CMPFLGS) lib/PCB.cpp

PQ.o: lib/PQ.cpp lib/PQ.h lib/Queue.h lib/PCB.h
	@$(CMP) $(CMPFLGS) lib/PQ.cpp

Queue.o: lib/Queue.cpp lib/Queue.h debug.h
	@$(CMP) $(CMPFLGS) lib/Queue.cpp

RTX.o: RTX.cpp RTX.h lib/PcbInfo.h lib/PCB.h 
	@$(CMP) $(CMPFLGS) RTX.cpp

SignalHandler.o: SignalHandler.cpp SignalHandler.h CCI.h
	@$(CMP) $(CMPFLGS) SignalHandler.cpp
	
Scheduler.o: Scheduler.cpp Scheduler.h lib/PQ.h lib/PCB.h
	@$(CMP) $(CMPFLGS) Scheduler.cpp
		
tests.o: tests.cpp tests.h debug.h
	@$(CMP) $(CMPFLGS) tests.cpp

TimingServices.o: TimingServices.cpp TimingServices.h
	@$(CMP) $(CMPFLGS) TimingServices.cpp

tools.o: tools.cpp tools.h
	@$(CMP) $(CMPFLGS) tools.cpp

userProcesses.o: userProcesses.cpp userProcesses.h
	@$(CMP) $(CMPFLGS) userProcesses.cpp

WallClock.o: lib/WallClock.cpp lib/WallClock.h
	@$(CMP) $(CMPFLGS) lib/WallClock.cpp

#external processes
keyboard.o: keyboard.cpp
	@$(CMP) $(CMPFLGS) keyboard.cpp

crt.o: crt.cpp
	@$(CMP) $(CMPFLGS) crt.cpp
