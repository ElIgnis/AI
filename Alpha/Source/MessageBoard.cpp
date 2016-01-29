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
	display_list.push_back(msg);
	
	if (display_list.size() > 5)
	{
		display_list.erase(display_list.begin());
	}
}
bool MessageBoard::GetMsg(string message)
{
	for (std::vector<Message*>::iterator itr = msg_list.begin(); itr != msg_list.end(); ++itr)
	{
		if ((*itr)->message == message)
		{
			display_list.push_back((*itr));

			if (display_list.size() > 5)
			{
				display_list.erase(display_list.begin());
			}

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
			display_list.push_back((*itr));

			if (display_list.size() > 5)
			{
				display_list.erase(display_list.begin());
			}

			msg_list.erase(itr);
			return true;
		}
	}
	return false;
}

vector<Message*> MessageBoard::GetMsgBoard(void)
{
	return display_list;
}