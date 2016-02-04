#ifndef STORE_MAN_H
#define STORE_MAN_H

#include "GameObject2D.h"
#include "SceneManager.h"
#include "SpriteAnimation.h"
#include "Vector2.h"
#include "MessageBoard.h"
#include <fstream>
#include <sstream>

using std::getline;

class StoreMan : public GameObject2D
{
public:
	enum STATES
	{
		S_IDLE = 0,
		S_NEWORDER,
		S_MOVEORDER,
		S_RCRUBBISHMAN,
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
	void setCurrentState(STATES newState);

	void Update(double dt, MessageBoard* mb, float* ingredient, float* trash, bool* orderarrived, bool* waitOrder);
	void UpdateFSM(MessageBoard* mb, bool* orderarrived, bool* waitOrder);
	void UpdateIdle(double dt);
	void UpdateMakeOrder(double dt, bool* waitOrder);
	void UpdateMoveOrder(double dt, float* ingredients);
	void UpdateRCRubbishMan(double dt, float* trash);
	void Draw(SceneManager* sceneManager);

	void ReadWayPoints_NewOrder(string fileName);
	void ReadWayPoints_MoveOrder(string fileName);
	void ReadWayPoints_RCRubbishMan(string fileName);
	void ReadWayPoints_StartPosition(string fileName);

	void AddWayPoints_NewOrder(Vector2 newWayPoint);
	void AddWayPoints_MoveOrder(Vector2 newWayPoint);
	void AddWayPoints_RCRubbishMan(Vector2 newWayPoint);

	bool UpdatePath(vector<Vector2> PathToUpdate, bool Reverse, double dt);
	bool UpdatePath(Vector2 PathToUpdate, bool Reverse, double dt);

	SpriteAnimation* GetSpriteAnim(void);
	SpriteAnimation* GetLegendSpriteAnim(void);
	void SetSpriteAnim(SpriteAnimation* NewSpriteAnim);

	void SetRC_RubbishMan(bool roleChanged);

	Vector2 GetPosition();
	void SetPosition(Vector2 newPos);

private:
	SpriteAnimation* spriteAnim;
	SpriteAnimation* spriteAnim_Legend;

	STATES currentState;
	bool m_bTaskFinish;
	bool m_bWaitingOrder;
	bool m_bPathAssigned;
	bool m_bRC_RubbishMan;

	int m_iNextPoint;

	float m_fDistSquared;
	float m_fMoveSpeed;

	Vector2 m_v2CurrentPos;
	Vector2 m_v2NextPos;
	Vector2 m_v2Direction;

	Vector2 Idle;
	vector<Vector2> NewOrder;
	vector<Vector2> MoveOrder;
	vector<Vector2> RCRubbishMan;

	//Text file reading
	int m_iReadLine;
	string WPData;
	char m_cSplit_Char;
};

#endif