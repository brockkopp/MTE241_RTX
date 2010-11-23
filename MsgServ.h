#ifndef H_MSG_SERV
#define H_MSG_SERV

#include "debug.h"
#include "RTX.h"
#include "lib/Queue.h"
#include "MsgTrace.h"
#include "lib/PCB.h"

class Scheduler;

class MsgServ
{
	public:
		MsgServ(Scheduler* scheduler);
		~MsgServ();
		int sendMsg(int destPid, MsgEnv* msg);
		MsgEnv* recieveMsg();
		MsgEnv* retrieveAck();
		int releaseEnv(MsgEnv* msg);
		MsgEnv* requestEnv();
	private:
		Queue* _freeEnvQ;
		MsgTrace* _msgTrace;
		Scheduler* _scheduler;
};

#endif
