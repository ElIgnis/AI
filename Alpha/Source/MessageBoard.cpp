#include "MessageBoard.h"


MessageBoard::MessageBoard()
{
}


MessageBoard::~MessageBoard()
{
}

void MessageBoard::AddMessage(string message, string sender, string receiver)
{
	Message msg;
	msg.message = message;
	msg.sender = sender;
	msg.receiver = receiver;

	msg_list.push_back(msg);
}
bool MessageBoard::GetMessage(string message)
{
	for (std::vector<Message>::iterator itr = msg_list.begin(); itr != msg_list.end(); ++itr)
	{
		if ((*itr).message == message)
		{
			return true;
		}
	}
	return false;
}