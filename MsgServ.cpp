#include "rtx.h"

class MsgServ
{
	public:
		int sendMsg(int destPid, MsgEnv* msg)
		MsgEnv* recieveMsg()
		int releaseEnv(MsgEnv* msg)
		MsgEnv* requestEnv()
	private:
		LL_List freeEnvQ;
};

int MsgServ::sendMsg(int destPid, MsgEnv* msg)
{
	//sends msg to PCB of destPID
	return -2;
}

MsgEnv* MsgServ::recieveMsg()
{
	//returns ptr to msg 
	return NULL;
}

int MsgServ::releaseEnv(MsgEnv* msg)
{
	//returns env to freeEnvQ
	return -2;
}

MsgEnv* MsgServ::requestEnv()
{
	//returns msg env from freeEnvQ
	return NULL;
}

