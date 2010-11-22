#include "MsgServ.h"

extern RTX* gRTX;

MsgServ::MsgServ(Scheduler* scheduler, MsgTrace* msgTrace)
{
	_scheduler = scheduler;
	_msgTrace = msgTrace;

	
	//allocate space for msg envelopes
	int msgTotal = 0;
	_freeEnvQ = new Queue(Queue::MSG_ENV);
	while(msgTotal < 20)
	{
		MsgEnv* msg = new MsgEnv;
		_freeEnvQ->enqueue(msg);
		msgTotal++;
	}
}

MsgServ::~MsgServ()
{
	//dealocate space from msg envelopes
	int msgTotal = 20;
	MsgEnv* msg;
	while(msgTotal > 0)
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
		debugMsg("Sending...\n"); //ERic
		PCB* tempPCB;	//ERic	
		
		assure(gRTX->getCurrentPcb(&tempPCB) == EXIT_SUCCESS,"Failed to retrieve current PCB",__FILE__,__LINE__,__func__,false); //ERic
		
		//insert destination and origin into msg envelope
		msg->setDestPid(destPid);

		debugMsg("\tdest set\n"); //ERic
		_msgTrace->addTrace(msg, SEND); 
		debugMsg("\ttrace added\n"); //ERic
		msg->setOriginPid(tempPCB->getId()); 
		debugMsg("\torigin set\n"); //ERic

		_msgTrace->addTrace(msg, SEND);  

		msg->setOriginPid(tempPCB->getId()); 
		

		//retrieve destination process PCB
		PCB* tempDestPCB;
		assure(gRTX->getPcb(destPid, &tempDestPCB) == EXIT_SUCCESS,"Failed to retrieve dest. PCB",__FILE__,__LINE__,__func__,false);
		//determine if process being sent to needs to be made ready
		bool temp;
		int tempStatus = tempDestPCB->getState();
		if(tempStatus == BLOCKED_MSG_RECIEVE)
			temp = _scheduler->unblock_process(tempDestPCB);
		else if(tempStatus == SLEEPING)
			if(msg->getMsgType() == "20")																		//wake_up
				temp = _scheduler->unblock_process(tempDestPCB);	
		
		if(!temp)//if unblocking did not work
			//return EXIT_ERROR; //ERic  not needed
			

		//add msg to process mailbox
		tempDestPCB->addMail(msg);

		return EXIT_SUCCESS;
	}
	return EXIT_ERROR;
}

MsgEnv* MsgServ::recieveMsg()
{
	//retrieve PCB of currently excecuting process 
	PCB* tempPCB;

	assure(gRTX->getCurrentPcb(&tempPCB) == EXIT_SUCCESS,"Failed to retrieve current PCB",__FILE__,__LINE__,__func__,false); //ERic
	//assure(gRTX->getPcb(6,&tempPCB) == EXIT_SUCCESS,"Failed to retrieve current PCB",__FILE__,__LINE__,__func__,false);  //ERic
	if (tempPCB->checkMail() == 0)

	{
		//i_process cannot be blocked
		if (tempPCB->getProcessType() == PROCESS_I)
    	return NULL;
  	//block calling process
		_scheduler->block_process(tempPCB, BLOCKED_MSG_RECIEVE); 		
		gRTX->K_release_processor();
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
	_freeEnvQ->enqueue(msg);
	debugMsg("length of free env Q: "+intToStr(_freeEnvQ->get_length())+"\n");  //ERic
	//check if another process is waiting for an envelope
	
	PCB* tempPcb = _scheduler->get_blocked_on_env(); 
	//unblock waiting process, if one is waiting   
	bool temp;
	if(tempPcb != NULL)
		temp = _scheduler->unblock_process(tempPcb);
	if(!temp)
		return EXIT_ERROR;
	 // ERic
	return EXIT_SUCCESS;
}

MsgEnv* MsgServ::requestEnv()
{

	if( _freeEnvQ->isEmpty() ) 
	{
		//retrieve PCB of currently excecuting process 
		PCB* tempPCB;
		assure(gRTX->getCurrentPcb(&tempPCB) == EXIT_SUCCESS,"Failed to retrieve current PCB",__FILE__,__LINE__,__func__,false);
		//i_process cannot be blocked
		if (tempPCB->getProcessType() == PROCESS_I)
    	return NULL;
		//block process is no envelope is available
 		_scheduler->block_process(tempPCB, BLOCKED_ENV); 			
		gRTX->K_release_processor();
	}
	MsgEnv* ptrMsg = _freeEnvQ->dequeue_MsgEnv();
	debugMsg("free env Q length: "+intToStr(_freeEnvQ->get_length())+"\n"); //ERic
	return ptrMsg;
}
