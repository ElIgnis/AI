#ifndef RUBBISH_MAN_H
#define RUBBISH_MAN_H

#include "GameObject2D.h"
#include "SceneManager.h"
#include "SpriteAnimation.h"
#include "Vector2.h"
#include <fstream>
#include <sstream>

using std::getline;

class RubbishMan : public GameObject2D
{
public:
	enum STATES
	{
		S_IDLE = 0,
		S_EAT,
		S_TAKETRASH,
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

	RubbishMan();
	~RubbishMan();

	void Init();

	STATES getCurrentState(void);

	void Update(double dt,int worldTime, float* trash);
	void UpdateFSM(int worldTime, float trash);
	void UpdateIdle(double dt);
	void UpdateTakeTrash(double dt, float* trash);
	void UpdateEat(double dt, int worldTime);
	void Draw(SceneManager* sceneManager);

	void ReadWayPoints_Eat(string fileName);
	void ReadWayPoints_TakeTrash(string fileName);
	void ReadWayPoints_StartPosition(string fileName);

	void AddWayPoints_Eat(Vector2 newWayPoint);
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
	bool m_bNeedToEat;
	bool m_bPathAssigned;
	bool m_bSeated;

	int m_iNextPoint;
	int m_iActionDuration;

	float m_fDistSquared;
	float m_fMoveSpeed;

	Vector2 m_v2CurrentPos;
	Vector2 m_v2NextPos;
	Vector2 m_v2Direction;

	Vector2 Idle;
	vector<Vector2> TakeTrash;
	vector<Vector2> Eat;

	//Text file reading
	int m_iReadLine;
	string WPData;
	char m_cSplit_Char;
};

#endif