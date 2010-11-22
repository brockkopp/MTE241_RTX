#ifndef H_MAILBOX
#define H_MAILBOX
#include "../debug.h"
#include "../tools.h"
#include "Queue.h"

class Queue;

class Mailbox
{
	public:
		Mailbox();
		~Mailbox();
		void empty();
		int getSize();
		int deliverMail(MsgEnv* newMsg);
		MsgEnv* getMail();
		MsgEnv* getMail(string msgType);
		MsgEnv* getMail(int msgType);
		
	private:
		Queue* _q;
};

#endif
