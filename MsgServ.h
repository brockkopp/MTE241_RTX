#ifndef H_MSG_SERV
#define H_MSG_SERV

#include "debug.h"
#include "RTX.h"
#include "lib/Queue.h"
#include "MsgTrace.h"
#include "lib/PCB.h"

#define BLOCKED_MSG_RECIEVE 2
#define SLEEPING 3							
#define ENV_NUMBER 20 //How many msg envelopes are available.

class Scheduler;
class MsgTrace;

class MsgServ
{
	public:
		MsgServ(Scheduler* scheduler, MsgTrace* msgTrace);
		~MsgServ();
		int sendMsg(int destPid, MsgEnv* msg);
		MsgEnv* recieveMsg();
		MsgEnv* retrieveOAck();
		int releaseEnv(MsgEnv* msg);
		MsgEnv* requestEnv();
//	private:
		Queue* _freeEnvQ;
		MsgTrace* _msgTrace;
		Scheduler* _scheduler;
};

#endif
