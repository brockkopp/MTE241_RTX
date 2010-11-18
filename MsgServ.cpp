#include "MsgServ.h"


static string WAKE_UP = "wake_up";

int MsgServ::sendMsg(int destPid, MsgEnv* msg)
{
	if(destPid >= 0 && destPid <= PROCESS_COUNT)
	{
		//retrieve PCB of currently excecuting process 
		PCB* tempPCB = _scheduler->get_current_process();
		
		//insert destination and origin into msg envelope
		msg->setDestPid(destPid);
		msg->setOriginPid(tempPCB->get_id()); 
		_msgTrace->addTrace(msg, SEND); //Q*Q*Q*Q*Q*Q*Q*Q*Q**Q*Q*Q*Q*Q*Q*Q should this be a pointer to a pointer to the envelope? &msg 

		//retrieve destination process PCB
		PCB* tempDestPCB;
		getPcb(destPid, &tempDestPCB);//is this right to send a PCB**? Q*Q*Q*Q*Q*Q*Q*Q*Q*Q*Q
		
		bool temp;
		int tempStatus = K_request_process_status(tempDestPCB);
		if(tempStatus == BLOCKED_MSG_RECIEVE)
			temp = _scheduler->unblock_process(tempDestPCB);
		else if(tempStatus == SLEEPING)
			if(msg->getMsgType == WAKE_UP)
				temp = _scheduler->unblock_process(tempDestPCB);	
		if(!temp)
			return EXIT_ERROR;
			
		tempDestPCB->add_mail(msg);
		return EXIT_SUCCESS;
	}
}

MsgEnv* MsgServ::recieveMsg()
{
	//retrieve PCB of currently excecuting process 
	PCB* tempPCB = _scheduler->get_current_process();
	
	if (tempPCB->check_mail() == 0)
	{
		//i_process cannot be blocked
		if (tempPCB->get_processType == PROCESS_I)
    	return NULL;
  	_scheduler->block_process(tempPCB, BLOCKED_MSG_RECEIVE); 
		_scheduler->process_switch(); 
	}
	MsgEnv* tempMsg = tempPCB->retrieve_mail();
	_msgTrace->addTrace(tempMsg, RECEIVE);
	
	return tempMsg;
}

int MsgServ::releaseEnv(MsgEnv* msg)
{
	/*
	if (msg == NULL)
		return EXIT_ERROR;
		
	freeEnvQ.enqueue(msg);
	PCB* tempPcb = is_blocked_on_envelope(); //need to find the function again. Location? this function returned a PCB   
	bool temp;
	if(tempPcb != NULL)
		temp = unblock_process(tempPcb);
	if(!temp)
		return EXIT_ERROR;
	*/
	return -2; //EXIT_SUCCESS;
}

MsgEnv* MsgServ::requestEnv()
{
	//retrieve PCB of currently excecuting process 
	PCB* tempPCB = _scheduler->get_current_process();
	
	if( freeEnvQ->isEmpty() ) 
	{
		//do i need to do something to prevent blocking an i_process Q*Q*Q*Q*Q*Q*Q*Q*Q
 		_scheduler->block_process(tempPCB, BLOCKED_ENV); 			
		_scheduler->process_switch(); 
	}
	MsgEnv* ptrMsg = freeEnvQ->dequeue_MsgEnv();
	return ptrMsg;
}
