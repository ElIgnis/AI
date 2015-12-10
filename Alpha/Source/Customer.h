#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "GameObject2D.h"
#include "SceneManager.h"
#include "Vector2.h"
#include "SpriteAnimation.h"

class Customer : GameObject2D
{
public: 
	enum STATES
	{
		S_IDLE,
		S_WALKING,
		S_DECIDE,
		S_QUEUE,
		S_BUY,
		S_WAIT,
		S_PICKUP,
		NUM_STATES
	};

	enum SPRITES
	{
		WALK_DOWN,
		WALK_LEFT,
		WALK_RIGHT,
		WALK_UP,
		NUM_SPRITES
	};

	Customer(Vector2 startPos);	//Default constructor
	~Customer();	//Default destructor

	void Update(double dt, int worldTime, int weather);	//Update based on state	

	STATES getCurrentState(void);	//Get current state 
	Vector2 getCurrentPos();	//Get current position
	void setStartPos(Vector2 startpos);	//Setting new start position 
	void setNextPoint(Vector2 nextpoint);	//Set next point to head towards

	bool getOutdoorStatus();	//Get where customer is (outdoor / indoor)
	void setActive(bool active);	//Set active status
	bool getActive();	//Get active status
	bool getQueueStatus();	//Get queue status

	bool getInQueueStatus();	//Get queue status
	void setInQueueStatus(bool inqueue);	//Set queue status
	bool getWaitStatus();	//Get wait status
	bool getInWaitStatus();	//Get in wait status
	void setInWaitStatus(bool wait);	//Set in wait status
	void setDrinkAvailable(bool available);	//set If drink available status

	Vector2 getPos();	//Get customer pos

	float getDelay();	//Get delay
	void setPickedUp(bool pickup);	//Set if drink has been picked up status

	void setOrderPlaced(bool placed);	//Get whether customer has placed order
	bool getOrderPlaced(void);	//Get whether customer has placed order

	bool CalculateProbability(int time, int weather);	//Calculating probability based on time, weather

	void setSprite(SpriteAnimation* sprite);	//Set current sprite
	SpriteAnimation* getSprite(void);	//Get current sprite

	STATES getState(void);	//Get current state

	void Reset(void);	//Reset variables
private:
	STATES currentState;	//Current state

	const float m_fSpeed = 500.f;	//Speed of movement
	float m_fDistanceSq;	//Distance squared
	Vector2 m_v2CurrentPos;	//Current position
	Vector2 m_v2NextPos;	//Next position

	Vector2 m_v2BuyPos;	//Buying position inside shop
	Vector2 m_v2ShopPos;	//Position of shop
	Vector2 m_v2WaitPos;	//Waiting position inside shop
	Vector2 m_v2PickupPos;	//Pick up position inside shop

	bool m_bActive;	//Active
	bool m_bDrinkAvailable;	//If drinks availabe to pick up
	bool m_bOutdoor;	//Outdoor / indoor status
	bool m_bQueue;	//Start queuing
	bool m_bInQueue;	//Inside queue
	bool m_bPickedUp;	//Picked up drink 
	bool m_bDeciding;	//Deciding boolean for feedback
	bool m_bWait;	//Start waiting
	bool m_bInWait;	//Inside waiting queue
	bool m_bOrderPlaced;	//Placed Order status
	float m_fDelay;	//Timer for delays

	SpriteAnimation* currentSprite;
};

#endif