#ifndef H_MSG_SERV
#define H_MSG_SERV

#include "debug.h"
#include "RTX.h"
#include "lib/Queue.h"
#include "MsgTrace.h"
#include "PCB.h"


class MsgServ
{
	public:
		int sendMsg(int destPid, MsgEnv* msg);
		MsgEnv* recieveMsg();
		int releaseEnv(MsgEnv* msg);
		MsgEnv* requestEnv();
	private:
		Queue* freeEnvQ;
};

#endif
