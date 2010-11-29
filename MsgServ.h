#ifndef H_MSG_SERV
#define H_MSG_SERV

#include "tools.h"
#include "debug.h"
#include "RTX.h"
#include "lib/Queue.h"
#include "MsgTrace.h"
#include "lib/PCB.h"			

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

	private:
		Queue* 			_freeEnvQ;
		MsgTrace* 	_msgTrace;
		Scheduler* 	_scheduler;
		MsgEnv*			_kernelEnv;
		bool				_sKerEnvInUse;
};

#endif
