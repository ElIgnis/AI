#include "MessageBoard.h"


MessageBoard::MessageBoard()
{
}


MessageBoard::~MessageBoard()
{
}

void MessageBoard::AddMessage(string message, string sender, string receiver)
{
	Message *msg = new Message;
	msg->message = message;
	msg->sender = sender;
	msg->receiver = receiver;

	msg_list.push_back(msg);
}
bool MessageBoard::GetMsg(string message)
{
	for (std::vector<Message*>::iterator itr = msg_list.begin(); itr != msg_list.end(); ++itr)
	{
		if ((*itr)->message == message)
		{
			msg_list.erase(itr);
			return true;
		}
	}
	return false;
}

bool MessageBoard::GetMsg(string message, string receiver)
{
	for (std::vector<Message*>::iterator itr = msg_list.begin(); itr != msg_list.end(); ++itr)
	{
		if ((*itr)->message == message && (*itr)->receiver == receiver)
		{
			msg_list.erase(itr);
			return true;
		}
	}
	
	return false;
}