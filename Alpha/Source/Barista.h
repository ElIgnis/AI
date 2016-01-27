#ifndef BARISTA_H
#define BARISTA_H

#include "GameObject2D.h"
#include "SceneManager.h"
#include "Vector2.h"
#include "Customer.h"
#include "MessageBoard.h"

#include <fstream>
#include <sstream>

using std::getline;

//Define messages to send here
#define MSG_DELIVERY_READY "Delivery items are ready"
#define MSG_LOW_INGREDIENTS "Ingredients are running low"
#define MSG_RUBBISH_FULL "Rubbish bin is full"
#define RC_TO_BARISTA "Too many customers"

class Barista : GameObject2D
{
public:
	enum STATES
	{
		S_IDLE = 0,
		S_REFILL,
		S_BREW,
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

	Barista();
	~Barista();

	void Init();

	STATES getCurrentState(void);

	void Update(double dt, float& ingredients, float& trash, float& reserve, MessageBoard* mb);
	void UpdateIdle(double dt, MessageBoard* mb);
	void UpdateRefill(double dt, float& ingredients, float& reserve, MessageBoard* mb);
	void UpdateBrew(double dt, float& ingredients, float& trash, MessageBoard* mb);
	void Draw(SceneManager* sceneManager);

	void ReadWayPoints_Refill(string fileName);
	void ReadWayPoints_Brew(string fileName);

	void AddWayPoints_Refill(Vector2 newWayPoint);
	void AddWayPoints_Brew(Vector2 newWayPoint);

	int GetNumDrinks(void);
	bool GetDrinkPrepared(void);
	void SubtractDrinkPrepared(void);

	void addNumOrders(const int numOrders);
	int getNumOrders(void);
	int getNumDrinksPrepared(void);

	void addNumDeliveryOrders(const int numOrders);
	int getNumDeliveryOrders(void);
	int getNumDeliveryPrepared(void);

	Vector2 GetPos();
	Vector2 GetDir();

	vector<Vector2> ReturnCurrentPath;
	bool UpdatePath(vector<Vector2> PathToUpdate, bool Reverse, double dt);

	SpriteAnimation* GetSpriteAnim(void);
	void SetSpriteAnim(SpriteAnimation* NewSpriteAnim);
	
private:
	SpriteAnimation* spriteAnim;

	STATES currentState;
	bool m_bNeedToRefill;
	bool m_bNeedToBrew;
	bool m_bPathAssigned;

	int m_iNextPoint;
	int m_iNumOrders;
	int m_iNumDeliveryOrders;
	int m_iBrewBar;
	int m_iDrinksPrepared;
	int m_iDeliveriesPrepared;

	float m_fBrewTimer;
	float m_fBrewProgress;
	float m_fRefillTimer;
	float m_fRefillProgress;
	float m_fMoveSpeed;			//Speed is decided by time and weather

	float m_fDistSquared;
	Vector2 m_v2CurrentPos;
	Vector2 m_v2NextPos;
	Vector2 m_v2Direction;

	vector<Vector2> Brew;
	vector<Vector2> Refill;

	//Text file reading
	int m_iReadLine;
	string WPData;
	char m_cSplit_Char;
};

#endif