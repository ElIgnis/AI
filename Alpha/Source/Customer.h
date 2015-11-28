#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "GameObject2D.h"
#include "SceneManager.h"
#include "Vector2.h"

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
		S_STAY,
		NUM_STATES
	};

	Customer(Vector2 startPos);	//Default constructor
	~Customer();	//Default destructor

	void Update(double dt, int worldTime, int weather);	//Update based on state
	void UpdateFSM();	

	STATES getCurrentState(void);	//Get current state 
	Vector2 getCurrentPos();	//Get current position
	void setStartPos(Vector2 startpos);	//Setting new start position 
	void setNextPoint(Vector2 nextpoint);	//Set next point to head towards

	bool getOutdoorStatus();	//Get where customer is (outdoor / indoor)
	void setActive(bool active);	//Set active status
	bool getActive();	//Get active status

	int CalculateProbability(int time, int weather);	//Calculating probability based on time, weather

private:
	STATES currentState;	//Current state

	const float m_fSpeed = 300.f;	//Speed of movement
	float m_fDistanceSq;	//Distance squared
	Vector2 m_v2CurrentPos;	//Current position
	Vector2 m_v2NextPos;	//Next position
	bool m_bActive;	//Active
	bool m_bOutdoor;	//Outdoor / indoor status
	Vector2 m_v2ShopPos;	//Position of shop
};

#endif