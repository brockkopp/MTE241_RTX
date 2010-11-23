#include "MsgServ.h"

//static string WAKE_UP = "wake_up";

extern RTX* gRTX;

MsgServ::MsgServ(Scheduler* scheduler)
{
	_scheduler = scheduler;
	_freeEnvQ = new Queue(Queue::MSG_ENV);
	
	int msgTotal = 0;
	while(msgTotal <= 20)
	{
		MsgEnv* msg = new MsgEnv;
		_freeEnvQ->enqueue(msg);
		msgTotal++;
	}
}

MsgServ::~MsgServ()
{
	int msgTotal = 20;
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
		_msgTrace->addTrace(msg, SEND);  

		msg->setOriginPid(tempPCB->getId()); 
		
		//retrieve destination process PCB
		PCB* tempDestPCB;
		gRTX->getPcb(destPid, &tempDestPCB);
		
		bool temp;
		int tempStatus = tempDestPCB->getState();
		if(tempStatus == BLOCKED_MSG_RECIEVE)
			temp = _scheduler->unblock_process(tempDestPCB);
		else if(tempStatus == SLEEPING)
			if(msg->getMsgType() == MsgEnv::WAKE_UP)
				temp = _scheduler->unblock_process(tempDestPCB);	
		if(!temp)
			return EXIT_ERROR;
			
		tempDestPCB->addMail(msg);
		return EXIT_SUCCESS;
	}
	return EXIT_ERROR;

}

MsgEnv* MsgServ::recieveMsg()
{
	//retrieve PCB of currently excecuting process 
	PCB* tempPCB;
	assure(gRTX->getCurrentPcb(&tempPCB) == EXIT_SUCCESS,"Failed to retrieve current PCB",__FILE__,__LINE__,__func__,false);
	
	if (tempPCB->checkMail() == 0)
	{
		//i_process cannot be blocked
		if (tempPCB->getProcessType() == PROCESS_I)
    		return NULL;
  		
		_scheduler->block_process(tempPCB, BLOCKED_MSG_RECIEVE); 		
		gRTX->K_release_processor();
	}
	MsgEnv* tempMsg = tempPCB->retrieveMail();
	_msgTrace->addTrace(tempMsg, RECEIVE);
	
	return tempMsg;
}

MsgEnv* MsgServ::retrieveAck()
{
	PCB* tempPCB;
	MsgEnv* ret;
	assure(gRTX->getCurrentPcb(&tempPCB) == EXIT_SUCCESS,"Failed to retrieve current PCB",__FILE__,__LINE__,__func__,false);
	
	//Attempt to retrieve a display acknowledgement
	ret = tempPCB->retrieveMail( MsgEnv::DISPLAY_ACK );
	//If no acknowledgements, search for display failure
	if(ret == NULL)
		ret = tempPCB->retrieveMail( MsgEnv::DISPLAY_FAIL );
	if(ret == NULL)
		ret = tempPCB->retrieveMail( MsgEnv::BUFFER_OVERFLOW );
	//return message (or NULL)
	return ret;
}

int MsgServ::releaseEnv(MsgEnv* msg)
{
	
	if (msg == NULL)
		return EXIT_ERROR;
		
	_freeEnvQ->enqueue(msg);
	PCB* tempPcb = _scheduler->get_blocked_on_env();    
	bool temp;
	if(tempPcb != NULL)
		temp = _scheduler->unblock_process(tempPcb);
	if(!temp)
		return EXIT_ERROR;
	
	return EXIT_SUCCESS;
}

MsgEnv* MsgServ::requestEnv()
{
	//retrieve PCB of currently excecuting process 
	PCB* tempPCB;
	assure(gRTX->getCurrentPcb(&tempPCB) == EXIT_SUCCESS,"Failed to retrieve current PCB",__FILE__,__LINE__,__func__,false);

	if( _freeEnvQ->isEmpty() ) 
	{
		//i_process cannot be blocked
		if (tempPCB->getProcessType() == PROCESS_I)
    	return NULL;

 		_scheduler->block_process(tempPCB, BLOCKED_ENV); 			
		gRTX->K_release_processor();
	}
	MsgEnv* ptrMsg = _freeEnvQ->dequeue_MsgEnv();
	return ptrMsg;
}
