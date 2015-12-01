#ifndef DELIVERYMAN_H
#define DELIVERYMAN_H

#include "GameObject2D.h"
#include "SceneManager.h"
#include "Vector2.h"

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

	STATES getCurrentState(void);

	void Update(double dt, int worldTime, int weather, bool order);
	void UpdateFSM(int worldTime, int weather, bool order);
	void Draw(SceneManager* sceneManager);

	//Outdoor controls
	bool getOutdoor(void);
	void setOutdoor(const bool isOutDoor);
	
	SpriteAnimation* GetSpriteAnim(void);
	void SetSpriteAnim(SpriteAnimation* NewSpriteAnim);

private:
	STATES currentState;
	bool m_bTaskFinish;
	bool m_bNeedToEat;
	bool m_bOutdoor;

	int m_iHoursNeeded;
	int m_iResult;
	int m_iDirection;

	float m_fJourneyTime;
	float m_fTotalTime;
	float m_fDelay;
	float m_fMoveSpeed;

	float m_fDistSquared;
	Vector2 m_v2CurrentPos;
	Vector2 m_v2NextPos;
};

#endif