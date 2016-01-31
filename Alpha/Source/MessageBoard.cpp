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

	string Display = msg->sender + " - \"" + msg->message +"\"" + " to " + msg->receiver;
	msg_list.push_back(msg);
	display_list.push_back(Display);
	
	if (display_list.size() > 5)
	{
		display_list.erase(display_list.begin());
	}
}

void MessageBoard::AddMessageOnce(string message, string sender, string receiver)
{
	Message *msg = new Message;
	msg->message = message;
	msg->sender = sender;
	msg->receiver = receiver;

	string Display = msg->sender + " - \"" + msg->message + "\"" + " to " + msg->receiver;

	bool messageExists = false;
	//Loop to find if message already exists
	for (std::vector<Message*>::iterator itr = msg_list.begin(); itr != msg_list.end(); ++itr)
	{
		if ((*itr)->message == message)
		{
			messageExists = true;
		}
	}

	//Removes message and not add it if exists
	if (messageExists)
	{
		delete msg;
		msg = NULL;
	}
	else
	{
		msg_list.push_back(msg);
		display_list.push_back(Display);

		if (display_list.size() > 5)
		{
			display_list.erase(display_list.begin());
		}
	}
}

bool MessageBoard::GetMsg(string message)
{
	for (std::vector<Message*>::iterator itr = msg_list.begin(); itr != msg_list.end(); ++itr)
	{
		if ((*itr)->message == message)
		{

			string Display = (*itr)->receiver + " Retrieved \"" + (*itr)->message + "\"";
			display_list.push_back(Display);

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
			string Display = (*itr)->receiver + " Retrieved \"" + (*itr)->message + "\"";
			display_list.push_back(Display);

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

vector<string> MessageBoard::GetDisplayBoard(void)
{
	return display_list;
}