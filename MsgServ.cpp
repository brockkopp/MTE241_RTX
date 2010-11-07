#include "MsgServ.h"

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
