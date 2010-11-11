#include "MsgServ.h"

int MsgServ::sendMsg(int destPid, MsgEnv* msg)
{
	if(destPid >= 0 && destPid <= PROCESS_COUNT)
	{
		//retrieve PCB of currently excecuting process 
		PCB* tempPCB = get_current_process();
		
		msg->setDestPid(destPid);
		msg->setOriginPid(tempPCB.get_id()); 
		addTrace(msg, SEND); //Q*Q*Q*Q*Q*Q*Q*Q*Q**Q*Q*Q*Q*Q*Q*Qshould this be a pointer to a pointer to the envelope?
		/*
		PCB* tempDestPCB = //need way to get PCB based on PID***********************************
		int tempStatus = K_request_process_status(tempDestPCB);//should require PCB?
		if(tempStatus == BLOCKED_MSG_RECIEVE)
		{
			bool temp = unblock_process(tempDestPCB);//need to know how to get PCB
			if(!temp)
				return EXIT_ERROR;
		}
		else if(tempStatus == SLEEPING)
			if(msg type == wake up)
			{
				bool temp = unblock_process(tempDestPCB);//need to know how to get PCB
				if(!temp)
					return EXIT_ERROR;
			}
		tempDestPCB.add_mail(msg);
		return EXIT_SUCCESS;
		*/
	return -2;
}

MsgEnv* MsgServ::recieveMsg()
{
	//retrieve PCB of currently excecuting process 
	PCB* tempPCB = get_current_process();
	
	if (tempPCB.check_mail() == 0)
	{
		//i_process cannot be blocked
		if (tempPCB.get_processType = PROCESS_I)
    	return NULL;
  	block_process(tempPCB, BLOCKED_MSG_RECEIVE); 
		process_switch(); //Q*Q*Q*Q*Q*Q*Q*Q*Q*Q*Q*Q*Q*Q*Q*Q*Q use release_processor?
	}
	MsgEnv* tempMsg = tempPCB.retrieve_mail();
	add_trace(tempMsg, RECEIVE);
	
	return tempMsg;
}

int MsgServ::releaseEnv(MsgEnv* msg)
{
	/*
	if (msg == NULL)
		return EXIT_ERROR;
		
	freeEnvQ.enqueue(msg);
	PCB* tempPcb = is_blocked_on_envelope(); //need to find the function again. Location? this function returned a PCB   
	if(tempPcb != NULL)
		bool temp = unblock_process(tempPcb);
	if(!temp)
		return EXIT_ERROR;
	*/
	return EXIT_SUCCESS;
}

MsgEnv* MsgServ::requestEnv()
{
	//retrieve PCB of currently excecuting process 
	PCB* tempPCB = get_current_process();
	
	if( freeEnvQ.isEmpty() ) 
	{
		//do i need to do something to prevent blocking an i_process Q*Q*Q*Q*Q*Q*Q*Q*Q
 		block_process(tempPCB, BLOCKED_ENV); 			
		process_switch(); //use release_processor?
	}
	MsgEnv* ptrMsg = freeEnvQ.dequeue_MsgEnv();
	return ptrMsg;
}
