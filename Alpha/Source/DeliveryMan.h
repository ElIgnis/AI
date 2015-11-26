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
		S_Sleeping,
		S_Eating,
		S_Delivering,
		S_Returning,
		NUM_STATES,
	};

	DeliveryMan();
	~DeliveryMan();

	void Update(double dt, int worldTime, bool order);
	void UpdateFSM(int worldTime, bool order);
	void Draw(SceneManager* sceneManager);

private:
	STATES currentState;
	bool taskFinish;
};

#endif