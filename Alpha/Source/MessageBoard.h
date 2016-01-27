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


class MessageBoard
{	
	struct Message{
		string sender, receiver, message;
	};
public:
	MessageBoard();
	~MessageBoard();

	//Function to Add A New Message to message board
	void AddMessage(string message, string sender, string receiver);

	//Function to get a message from message board
	bool GetMsg(string message);
	bool GetMsg(string message, string receiver);

private:
	vector<Message*> msg_list;

};

