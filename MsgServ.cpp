#include "MsgServ.h"

int MsgServ::sendMsg(int destPid, MsgEnv* msg)
{
	if(destPid >= 0 && destPid <= PROCESS_COUNT)
	{
		msg->setDestPid(destPid);
		msg->setOriginPid(0); //is there a way to access the PCB of the current process?
		addTrace(msg, SEND); //should this be a pointer to a pointer to the envelope?
		/*
		int tempStatus = K_request_process_status(destPid);
		if(tempStatus == BLOCKED_MSG_RECIEVE)
		{
			bool temp = unblock_process(PCB);//need to know how to get PCB
			if(!temp)
				return EXIT_ERROR;
		}
		else if(tempStatus == SLEEPING)
			if(msg type == wake up)
			{
				bool temp = unblock_process(PCB);//need to know how to get PCB
				if(!temp)
					return EXIT_ERROR;
			}
		enQ msg onto PCB
		*/
	return -2;
}

MsgEnv* MsgServ::recieveMsg()
{
/*
	if (PCB mailbox is empty)
	{
  	block_process(process_id, BLOCKED_ON_RECEIVE);
		process_switch(); //use release_processor?
	}
	tempMsg = dequeue msg env from PCB mailbox
	add_trace(tempMsg, RECEIVE);
	if (current_process = i_processA)
    return NULL;
	
	return pointer to msg env
*/
	return NULL;
}

int MsgServ::releaseEnv(MsgEnv* msg)
{
	/*
	if (msg == NULL)
		return EXIT_ERROR;
		
	free_env_Q.enqueue(msg);
	PCB* tempPtr = is_blocked_on_envelope();    
	if(tempPtr != NULL)
		bool temp = unblock_process(tempPtr);
	if(!temp)
		return EXIT_ERROR;
	*/
	return EXIT_SUCCESS;
}

MsgEnv* MsgServ::requestEnv()
{
	/*
	if (free_env_Q.isEmpty() ) 
	{
 		block_process(process_id, BLOCKED ON ENV) 			
		process_switch()
	}
	MsgEnv* ptrMsg = free_env_Q.dequeue_MsgEnv();
	return (ptrMsg)
	*/
	return NULL;
}
