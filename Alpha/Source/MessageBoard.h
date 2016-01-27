#pragma once

#include <string>
#include <vector>
#include <iostream>

using std::string;
using std::vector;

//Define AI roles
#define ROLE_BARISTA "Barista"
#define ROLE_DELIVERYMAN "Deliveryman"
#define ROLE_RUBBISHMAN "Rubbishman"
#define ROLE_STOREMAN "Storeman"

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
	bool GetMsg(string message);
	bool GetMsg(string message, string receiver);

};

