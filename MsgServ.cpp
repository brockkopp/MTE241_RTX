#include "MsgServ.h"

static string WAKE_UP = "wake_up";

extern RTX* gRTX;

MsgServ::MsgServ(Scheduler* scheduler)
{
	_scheduler = scheduler;
	
	int msgTotal = 0;
	while(msgTotal <= 10)
	{
		MsgEnv* msg = new MsgEnv;
		_freeEnvQ->enqueue(msg);
		msgTotal++;
	}
}

MsgServ::~MsgServ()
{
	int msgTotal = 10;
	MsgEnv* msg;
	while(msgTotal >= 0)
	{
		msg = _freeEnvQ->dequeue_MsgEnv();
		delete msg;
		msgTotal--;
	}
}

int MsgServ::sendMsg(int destPid, MsgEnv* msg)
{
	if(destPid >= 0 && destPid <= PROCESS_COUNT)
	{
		//retrieve PCB of currently excecuting process 
		
		PCB* tempPCB;

		assure(gRTX->getCurrentPcb(&tempPCB) == EXIT_SUCCESS,"Failed to retrieve current PCB",__FILE__,__LINE__,__func__,false);

		
		//insert destination and origin into msg envelope
		msg->setDestPid(destPid);
		_msgTrace->addTrace(msg, SEND); //Q*Q*Q*Q*Q*Q*Q*Q*Q**Q*Q*Q*Q should this be a pointer to a pointer to the envelope? &msg 

		msg->setOriginPid(tempPCB->get_id()); 
		
		//retrieve destination process PCB
		PCB* tempDestPCB;
		gRTX->getPcb(destPid, &tempDestPCB);//is this right to send a PCB**? Q*Q*Q*Q*Q*Q*Q*Q*Q*Q*Q
		
		bool temp;
		int tempStatus = tempDestPCB->get_state();//// issue with msg vs pcb
		if(tempStatus == BLOCKED_MSG_RECIEVE)
			temp = _scheduler->unblock_process(tempDestPCB);
		else if(tempStatus == SLEEPING)
			if(msg->getMsgType() == MsgEnv::WAKE_UP)
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
	PCB* tempPCB;
	assure(gRTX->getCurrentPcb(&tempPCB) == EXIT_SUCCESS,"Failed to retrieve current PCB",__FILE__,__LINE__,__func__,false);
	
	if (tempPCB->check_mail() == 0)
	{
		//i_process cannot be blocked
		if (tempPCB->get_processType() == PROCESS_I)
    	return NULL;
  		
		//block_process(tempPCB, BLOCKED_MSG_RECIEVE); 		//should probably be moved to RTX primitive to allow access to scheduler -Brock
		gRTX->K_release_processor();
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
		
	_freeEnvQ.enqueue(msg);
	PCB* tempPcb = is_blocked_on_envelope(); //need to find the function again. Location? this function returned a PCB   
	bool temp;
	if(tempPcb != NULL)
		temp = _scheduler->unblock_process(tempPcb);
	if(!temp)
		return EXIT_ERROR;
	*/
	return -2; //EXIT_SUCCESS;
}

MsgEnv* MsgServ::requestEnv()
{
	//retrieve PCB of currently excecuting process 
	PCB* tempPCB;
	assure(gRTX->getCurrentPcb(&tempPCB) == EXIT_SUCCESS,"Failed to retrieve current PCB",__FILE__,__LINE__,__func__,false);

	
	if( _freeEnvQ->isEmpty() ) 
	{
		//do i need to do something to prevent blocking an i_process Q*Q*Q*Q*Q*Q*Q*Q*Q
 		//block_process(tempPCB, BLOCKED_ENV); 			
		gRTX->K_release_processor();
	}
	MsgEnv* ptrMsg = _freeEnvQ->dequeue_MsgEnv();
	return ptrMsg;
}
