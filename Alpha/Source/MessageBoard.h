#pragma once

#include <string>
#include <vector>
#include <iostream>

using std::string;
using std::vector;

struct Message{
	string sender, receiver, message;
};

class MessageBoard
{
private:
	vector<Message>msg_list;
	
public:
	MessageBoard();
	~MessageBoard();
	void AddMessage(string message, string sender, string receiver);
	bool GetMessage(string message);
};

