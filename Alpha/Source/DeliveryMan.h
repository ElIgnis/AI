#ifndef DELIVERYMAN_H
#define DELIVERYMAN_H

#include "GameObject2D.h"
#include "SceneManager.h"


class DeliveryMan : GameObject2D
{
public:
	enum STATES
	{
		S_IDLE = 0,
		S_SLEEPING,
		S_EATING,
		S_DELIVERING,
		S_RETURNING,
		NUM_STATES,
	};

	DeliveryMan();
	~DeliveryMan();

	void Update(double dt, int worldTime, int weather, bool order);
	void UpdateFSM(int worldTime, int weather, bool order);
	void Draw(SceneManager* sceneManager);
	STATES getCurrentState(void);

private:
	STATES currentState;
	bool taskFinish;

	int m_iHoursNeeded;
	int m_iResult;
	float m_fJourneyTime;
	float m_fTotalTime;
};

#endif