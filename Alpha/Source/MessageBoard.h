#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <ostream>

using std::string;
using std::vector;
using std::ostream;

//Define AI roles
#define ROLE_BARISTA "Barista"
#define ROLE_DELIVERYMAN "Deliveryman"
#define ROLE_RUBBISHMAN "Rubbishman"
#define ROLE_STOREMAN "Storeman"

//Define messages
#define MSG_DELIVERY_READY "Delivery items are ready"
#define MSG_LOW_INGREDIENTS "Ingredients are running low"
#define MSG_RUBBISH_FULL "Rubbish bin is full"
#define RC_TO_BARISTA "Too many customers"
#define RC_TO_DELIVERYMAN "Too many orders"

struct Message{
	string sender, receiver, message;
};

class MessageBoard
{	
public:
	MessageBoard();
	~MessageBoard();

	//Function to Add A New Message to message board
	void AddMessage(string message, string sender, string receiver);

	//Function to get a message from message board
	bool GetMsg(string message);
	bool GetMsg(string message, string receiver);
	vector<string> GetDisplayBoard(void);

private:
	vector<Message*> msg_list;
	vector<string> display_list;
};

