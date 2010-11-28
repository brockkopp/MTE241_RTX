#include "MsgServ.h"

extern RTX* gRTX;

//CONSTRUCTOR
//Initializes queue of free envelopes and passes scheduler and msgTrace to the MsgServ class 
MsgServ::MsgServ(Scheduler* scheduler, MsgTrace* msgTrace)
{
	_scheduler = scheduler;
	_msgTrace = msgTrace;
	_freeEnvQ = new Queue(Queue::MSG_ENV);
	
	for(int i=0; i < MSG_COUNT; i++)
	{
		MsgEnv* temp = new MsgEnv();
		_freeEnvQ->enqueue( (void**)(&temp) );
	}
#if DEBUG_MODE
	_envelopeTracker = new Queue(Queue::TRACKER);
#endif

}

//DECONSTRUCTOR
//Frees memory allocated to msg envelopes 
MsgServ::~MsgServ()
{
	delete _freeEnvQ;

#if DEBUG_MODE
	delete _envelopeTracker;
#endif
}

//Facilitates the sending of messages between processes
// This function is non-blocking 
int MsgServ::sendMsg(int destPid, MsgEnv* msg)
{
	if(msg != NULL && destPid >= 0 && destPid < PROCESS_COUNT)
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
		assure(gRTX->getPcb(destPid, &tempDestPCB) == EXIT_SUCCESS,"Failed to retrieve dest. PCB",__FILE__,__LINE__,__func__,true);
		
		int status = tempDestPCB->getState();
		if(status == BLOCKED_MSG_RECIEVE || 
			 ( status == SLEEPING && msg->getMsgType() == MsgEnv::REQ_DELAY ))
		{
			_scheduler->unblock_process(tempDestPCB);
		}
		
		//determine if process being sent to needs to be made ready
		//add msg to process mailbox

		if( msg->getDestPid() != PROC_TIMING &&
			  msg->getMsgType() == MsgEnv::REQ_DELAY )
		{
			int type;
			strToInt(msg->getMsgData(),&type);
			msg->setMsgType(type);
			msg->setMsgData("");
		}

		tempDestPCB->addMail(msg);

		return EXIT_SUCCESS;
	}
	return EXIT_ERROR;
}
		
//Facilitates the retrieval of messages from the process PCB mailbox 
//This Function is blocking if there are no messages waiting to be recieved
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
  	if(gRTX->getCurrentPcb()->getState() != SLEEPING)
			_scheduler->block_process(BLOCKED_MSG_RECIEVE); 		
	}
	//get mail
	MsgEnv* tempMsg = tempPCB->retrieveMail();
	
	_msgTrace->addTrace(tempMsg, RECEIVE);
	return tempMsg;
}

//Returns an unneeded message envelope to the free envelope queue
//This function also checks if another process is waiting for a message envelope and unblocks if necessary
int MsgServ::releaseEnv(MsgEnv* msg)
{
cout << "CurrProc: " << gRTX->getCurrentPcb()->getName() << " freeEnvQ length before release: " << _freeEnvQ->get_length() << "\n";
	if (msg == NULL)
		return EXIT_ERROR;
		
	#if DEBUG_MODE
//		_envelopeTracker->pluck_Track(msg);
	#endif
		
	//return envelope to _freeEnvQ
	_freeEnvQ->enqueue((void**)(&msg));
 
	//unblock waiting process, if one is waiting   
	int temp;
	temp = _scheduler->unblock_process( BLOCKED_ENV );
	
cout << "CurrProc: " << gRTX->getCurrentPcb()->getName() << " freeEnvQ length after release: " << _freeEnvQ->get_length() << "\n";
	
	
	if(!temp)
		return EXIT_ERROR;
		
	return EXIT_SUCCESS;
}

//Provides the calling process with a message envelope from the free envelope queue 
//If the queue is empty, the process is blocked
MsgEnv* MsgServ::requestEnv()
{

cout << "CurrProc: " << gRTX->getCurrentPcb()->getName() << " freeEnvQ length before request: " << _freeEnvQ->get_length() << "\n";
	while( _freeEnvQ->isEmpty() ) 
	{
cout << "CurrProc: " << gRTX->getCurrentPcb()->getName() << "   Queue is empty!\n";
		debugMsg("Empty Envelope Queue!!!",1,1);
		//retrieve PCB of currently excecuting process 
		PCB* tempPCB = gRTX->getCurrentPcb();
		assure(tempPCB != NULL,"Failed to retrieve current PCB",__FILE__,__LINE__,__func__,false);
		//i_process cannot be blocked
		if (tempPCB->getProcessType() == PROCESS_I)
    		return NULL;
	
		//block process if no envelope is available. This chains into a context switch.
 		_scheduler->block_process(BLOCKED_ENV); 			
	}
			
	MsgEnv* ptrMsg = _freeEnvQ->dequeue_MsgEnv();
	
	#if DEBUG_MODE
//		envTrack* eT = new envTrack();
//		eT->allocatorID = gRTX->getCurrentPid();
//		eT->receiverID = -1;
//		MsgEnv* tMsg = ptrMsg;
//		eT->address = tMsg;
//		_envelopeTracker->enqueue(eT);
	#endif
		
cout << "CurrProc: " << gRTX->getCurrentPcb()->getName() << "freeEnvQ length after request: " << _freeEnvQ->get_length() << "\n";
		
		
	return ptrMsg;
}

#if DEBUG_MODE
void MsgServ::readTracker()
{
	_envelopeTracker->printTracker();
}
#endif
