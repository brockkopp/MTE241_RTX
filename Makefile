CMP=g++
CMPFLGS= -g -c -Wall	#compile only, show all warnings
LNKFLGS= -o
TITLE=RTX.out
OBJ= debug.o initialize.o iprocesses.o RTX.o SignalHandler.o Scheduler.o CCI.o MsgEnv.o MsgServ.o MsgTrace.o WallClock.o Queue.o PQ.o PCB.o userProcesses.o tools.o
KB=KB.out
KB_OBJ=keyboard.o debug.o
CRT=CRT.out
CRT_OBJ=crt.o debug.o

##Command List
all: compileMsg preclean $(KB) $(CRT) $(TITLE) clean noRunMsg

compileMsg:
	@echo
	@echo Starting compilation process...
	@echo

noRunMsg:
	@echo
	@echo Execute '"make run"' to compile and execute
	@echo

run: all execute

preclean: clean_all
	@rm -f $(TITLE) $(KB) $(CRT) $(OBJ) $(KB_OBJ) $(CRT_OBJ) *.buf busyFlag

clean:
	@rm -f $(OBJ) $(KB_OBJ) $(CRT_OBJ) #Delete all object files

clean_all:
	@rm -f *.o *.out #Delete everything!!!!

memcheck: all
	@valgrind --leak-check=yes --track-origins=yes --trace-children=yes ./RTX.out

execute:
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

debug.o: debug.cpp debug.h
	@$(CMP) $(CMPFLGS) debug.cpp

initialize.o: initialize.cpp debug.h RTX.h CCI.h lib/PcbInfo.h Shmem.h
	@$(CMP) $(CMPFLGS) initialize.cpp
	
iprocesses.o: iprocesses.cpp RTX.h SignalHandler.h Shmem.h lib/Queue.h
	@$(CMP) $(CMPFLGS) iprocesses.cpp

Mailbox.o: lib/Mailbox.cpp lib/Mailbox.h lib/Queue.h
	@$(CMP) $(CMPFLGS) lib/Mailbox.cpp
	
MsgEnv.o: lib/MsgEnv.cpp lib/MsgEnv.h
	@$(CMP) $(CMPFLGS) lib/MsgEnv.cpp
	
MsgServ.o:MsgServ.cpp MsgServ.h MsgTrace.h
	@$(CMP) $(CMPFLGS) MsgServ.cpp
	
MsgTrace.o:MsgTrace.cpp MsgTrace.h
	@$(CMP) $(CMPFLGS) MsgTrace.cpp
	
PCB.o: lib/PCB.cpp lib/PCB.h lib/PcbInfo.h
	@$(CMP) $(CMPFLGS) lib/PCB.cpp

PQ.o: lib/PQ.cpp lib/PQ.h lib/Queue.h lib/PCB.h
	@$(CMP) $(CMPFLGS) lib/PQ.cpp

Queue.o: lib/Queue.cpp lib/Queue.h debug.h
	@$(CMP) $(CMPFLGS) lib/Queue.cpp

RTX.o: RTX.cpp RTX.h lib/PcbInfo.h lib/PCB.h MsgServ.h Scheduler.h
	@$(CMP) $(CMPFLGS) RTX.cpp

SignalHandler.o: SignalHandler.cpp SignalHandler.h CCI.h
	@$(CMP) $(CMPFLGS) SignalHandler.cpp
	
Scheduler.o: Scheduler.cpp Scheduler.h lib/PQ.h lib/PCB.h
	@$(CMP) $(CMPFLGS) Scheduler.cpp

tools.o: tools.cpp tools.h
	@$(CMP) $(CMPFLGS) tools.cpp

userProcesses.o: userProcesses.cpp userProcesses.h
	@$(CMP) $(CMPFLGS) userProcesses.cpp

WallClock.o: lib/WallClock.cpp lib/WallClock.h
	@$(CMP) $(CMPFLGS) lib/WallClock.cpp

#external processes
keyboard.o: keyboard.cpp Shmem.h
	@$(CMP) $(CMPFLGS) keyboard.cpp

crt.o: crt.cpp Shmem.h
	@$(CMP) $(CMPFLGS) crt.cpp
