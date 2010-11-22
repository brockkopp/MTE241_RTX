#include "Mailbox.h"

Mailbox::Mailbox()
{
	_q = new Queue(Queue::MSG_ENV);
}
Mailbox::~Mailbox()
{
	if(_q != NULL)
		delete _q;
}
void Mailbox::empty()
{
	MsgEnv* tmp = NULL;
	do
	{
		tmp = _q->dequeue_MsgEnv();
	}
	while(tmp != NULL);
}
int Mailbox::getSize()
{
	return _q->get_length();
}
MsgEnv* Mailbox::getMail()
{
	return _q->dequeue_MsgEnv();
}
int Mailbox::deliverMail(MsgEnv* newMsg)
{
	return _q->enqueue(newMsg);
}
MsgEnv* Mailbox::getMail(int msgType)
{
	return getMail(intToStr(msgType));
}
MsgEnv* Mailbox::getMail(string msgType)
{
	QueueNode* curr = _q->_rear;
	QueueNode* prev = _q->_rear;
	MsgEnv* retMsg = NULL;

	while(curr != NULL)
	{
		retMsg = (MsgEnv*)curr->item;
		if( retMsg->getMsgType().compare(msgType) )
		{
			if(_q->_length == 1)
			{
				_q->_rear = NULL;
				_q->_front = NULL;
			}
			else
			{
				if(curr == _q->_rear)
					_q->_rear = curr->link;
				else if(curr == _q->_front)
					_q->_front = prev;
					
				prev->link = curr->link;
			}
			_q->_length--;
			break;
		}
		prev = curr;
		curr = curr->link;
	}
	return retMsg; //if value wasn't in queue, pluckee is NULL
}
