#include "MsgServ.h"

extern RTX* gRTX;

int MsgServ::sendMsg(int destPid, MsgEnv* msg)
{
	if(destPid >= 0 && destPid <= PROCESS_COUNT)
	{
		//retrieve PCB of currently excecuting process 
		
		PCB* tempPCB;

		assure(gRTX->getCurrentPcb(&tempPCB) == EXIT_SUCCESS,"Failed to retrieve current PCB",__FILE__,__LINE__,__func__,false);
		
		msg->setDestPid(destPid);
		//msg->setOriginPid(tempPCB.get_id()); 
		//addTrace(msg, SEND); //Q*Q*Q*Q*Q*Q*Q*Q*Q**Q*Q*Q*Q*Q*Q*Qshould this be a pointer to a pointer to the envelope?

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
	}
	return -2;
}

MsgEnv* MsgServ::recieveMsg()
{
	//retrieve PCB of currently excecuting process 
	PCB* tempPCB;
	assure(gRTX->getCurrentPcb(&tempPCB) == EXIT_SUCCESS,"Failed to retrieve current PCB",__FILE__,__LINE__,__func__,false);
	
	if (tempPCB->check_mail() == 0)
	{
		//i_process cannot be blocked
		if (tempPCB->get_processType() == PROCESS_I)
    	return NULL;
  		
		//block_process(tempPCB, BLOCKED_MSG_RECIEVE); 		//should probably be moved to RTX primitive to allow access to scheduler -Brock
		//process_switch(); //Q*Q*Q*Q*Q*Q*Q*Q*Q*Q*Q*Q*Q*Q*Q*Q*Q use release_processor?	//should probably be moved to RTX primitive to allow access to scheduler -Brock
	}
	MsgEnv* tempMsg = tempPCB->retrieve_mail();
	//add_trace(tempMsg, RECEIVE);
	
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
	PCB* tempPCB;
	assure(gRTX->getCurrentPcb(&tempPCB) == EXIT_SUCCESS,"Failed to retrieve current PCB",__FILE__,__LINE__,__func__,false);
	
	if( freeEnvQ->isEmpty() ) 
	{
		//do i need to do something to prevent blocking an i_process Q*Q*Q*Q*Q*Q*Q*Q*Q
 		//block_process(tempPCB, BLOCKED_ENV); 			
		//process_switch(); //use release_processor?
	}
	MsgEnv* ptrMsg = freeEnvQ->dequeue_MsgEnv();
	return ptrMsg;
}
