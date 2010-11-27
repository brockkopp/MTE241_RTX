#include "MsgServ.h"

extern RTX* gRTX;

MsgServ::MsgServ(Scheduler* scheduler, MsgTrace* msgTrace)
{
	_scheduler = scheduler;
	_msgTrace = msgTrace;

	_freeEnvQ = new Queue(Queue::MSG_ENV);
	
	for(int i=0; i < MSG_COUNT; i++)
		_freeEnvQ->enqueue( new MsgEnv() );
}

MsgServ::~MsgServ()
{
	delete _freeEnvQ;
}

int MsgServ::sendMsg(int destPid, MsgEnv* msg)
{
	if(destPid >= 0 && destPid <= PROCESS_COUNT)
	{
		//retrieve PCB of currently excecuting process 
		PCB* tempPCB;		
		assure((tempPCB = gRTX->getCurrentPcb()) != NULL,"Failed to retrieve current PCB",__FILE__,__LINE__,__func__,false); //ERic
		
		//insert destination and origin into msg envelope
		msg->setDestPid(destPid);
		msg->setOriginPid(tempPCB->getId());
		_msgTrace->addTrace(msg, SEND); 
 
		//retrieve destination process PCB
		PCB* tempDestPCB;
		assure(gRTX->getPcb(destPid, &tempDestPCB) == EXIT_SUCCESS,"Failed to retrieve dest. PCB",__FILE__,__LINE__,__func__,false);
		//determine if process being sent to needs to be made ready
		bool temp;
		int tempStatus = tempDestPCB->getState();
		if(tempStatus == BLOCKED_MSG_RECIEVE)
			temp = _scheduler->unblock_process(tempDestPCB);
		else if(tempStatus == SLEEPING)
			if(msg->getMsgType() == 20)																		//wake_up
				temp = _scheduler->unblock_process(tempDestPCB);				

		//add msg to process mailbox
		tempDestPCB->addMail(msg);

		return EXIT_SUCCESS;
	}
	return EXIT_ERROR;
}

MsgEnv* MsgServ::recieveMsg()
{

	//retrieve PCB of currently excecuting process 

	PCB* tempPCB = gRTX->getCurrentPcb();
	assure(tempPCB != NULL, "Failed to retrieve current PCB",__FILE__,__LINE__,__func__,false);

	if (tempPCB->checkMail() == 0)
	{
		//i_process cannot be blocked
		if (tempPCB->getProcessType() == PROCESS_I)
    		return NULL;
  		
  		//block calling process, this automatically calls a process_switch
		_scheduler->block_process(BLOCKED_MSG_RECIEVE); 		

	}
	//get mail
	MsgEnv* tempMsg = tempPCB->retrieveMail();
	_msgTrace->addTrace(tempMsg, RECEIVE);
	return tempMsg;
}

int MsgServ::releaseEnv(MsgEnv* msg)
{
	if (msg == NULL)
		return EXIT_ERROR;
		
	//return envelope to _freeEnvQ
	msg->initMsg(-1,-1,-1,"");
	_freeEnvQ->enqueue(msg);

 
 
	//unblock waiting process, if one is waiting   
	int temp;
	temp = _scheduler->unblock_process( BLOCKED_ENV );
	if(!temp)
		return EXIT_ERROR;
		
	return EXIT_SUCCESS;
}

MsgEnv* MsgServ::requestEnv()
{
	while( _freeEnvQ->isEmpty() ) 
	{
		debugMsg("Empty Envelope Queue!!!",1,1);
		//retrieve PCB of currently excecuting process 
		PCB* tempPCB;
		assure((tempPCB = gRTX->getCurrentPcb()) != NULL,"Failed to retrieve current PCB",__FILE__,__LINE__,__func__,false);
		//i_process cannot be blocked
		if (tempPCB->getProcessType() == PROCESS_I)
    		return NULL;
	
		//block process if no envelope is available. This chains into a context switch.
 		_scheduler->block_process(BLOCKED_ENV); 			
	}
	MsgEnv* ptrMsg = _freeEnvQ->dequeue_MsgEnv();
	return ptrMsg;
}
