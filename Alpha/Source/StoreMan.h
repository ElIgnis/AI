#ifndef STORE_MAN_H
#define STORE_MAN_H

#include "GameObject2D.h"
#include "SceneManager.h"
#include "SpriteAnimation.h"
#include "Vector2.h"
#include <fstream>
#include <sstream>

using std::getline;

class StoreMan : GameObject2D
{
public:
	enum STATES
	{
		S_IDLE = 0,
		S_NEWORDER,
		S_MOVEORDER,
		NUM_STATES,
	};

	enum POS
	{
		X = 0,
		Y,
		POS_INDEX,
	};

	enum SPRITES
	{
		WALK_DOWN,
		WALK_LEFT,
		WALK_RIGHT,
		WALK_UP,
		NUM_SPRITES,
	};
	
	StoreMan();
	~StoreMan();

	void Init();

	STATES getCurrentState(void);

	void Update(double dt, float* ingredient, bool* orderarrived, bool* waitOrder);
	void UpdateFSM(float ingredients, bool* orderarrived, bool* waitOrder);
	void UpdateIdle(double dt);
	void UpdateMakeOrder(double dt, bool* waitOrder);
	void UpdateMoveOrder(double dt, float* ingredients);
	void Draw(SceneManager* sceneManager);

	void ReadWayPoints_NewOrder(string fileName);
	void ReadWayPoints_MoveOrder(string fileName);
	void ReadWayPoints_TakeTrash(string fileName);
	void ReadWayPoints_StartPosition(string fileName);

	void AddWayPoints_NewOrder(Vector2 newWayPoint);
	void AddWayPoints_MoveOrder(Vector2 newWayPoint);
	void AddWayPoints_TakeTrash(Vector2 newWayPoint);

	bool UpdatePath(vector<Vector2> PathToUpdate, bool Reverse, double dt);
	bool UpdatePath(Vector2 PathToUpdate, bool Reverse, double dt);

	SpriteAnimation* GetSpriteAnim(void);
	void SetSpriteAnim(SpriteAnimation* NewSpriteAnim);

	Vector2 GetPosition();

private:
	SpriteAnimation* spriteAnim;

	STATES currentState;
	bool m_bTaskFinish;
	bool m_bWaitingOrder;
	bool m_bPathAssigned;

	int m_iNextPoint;

	float m_fDistSquared;
	float m_fMoveSpeed;

	Vector2 m_v2CurrentPos;
	Vector2 m_v2NextPos;
	Vector2 m_v2Direction;

	Vector2 Idle;
	vector<Vector2> NewOrder;
	vector<Vector2> MoveOrder;

	//Text file reading
	int m_iReadLine;
	string WPData;
	char m_cSplit_Char;
};

#endif