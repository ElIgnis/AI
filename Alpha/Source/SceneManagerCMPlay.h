#ifndef SCENE_MANAGER_CM_PLAY_H
#define SCENE_MANAGER_CM_PLAY_H

#include "SceneManagerGameplay.h"
#include "SceneNode.h"
#include "DeliveryMan.h"
#include "Customer.h"
#include "Application.h"
#include "Barista.h"
#include "StoreMan.h"
#include "RubbishMan.h"

struct CustomerShopWaypoints
{
	string name;
	Vector2 waypoint;
};

class SceneManagerCMPlay : public SceneManagerGameplay
{
public:
	SceneManagerCMPlay();
	~SceneManagerCMPlay();

	void Init(const int width, const int height, ResourcePool* RP, InputManager* controls);
	void Update(double dt);
	void Render();
	void Exit();

	void BindShaders();
	void InitShader();
	void InitLight();
	void InitCamera();

	void RenderLight();
	void RenderBG();
	void RenderStaticObject();
	void RenderMobileObject();
	void RenderWaypoints();	//Render waypoints for debug purposes
	void RenderUIInfo();

	void FetchCustomer();	//Set new customer at spawn

	void UpdateGoodsDelivery(double dt); // Update the goods delivery status

private:
	const float m_fBGpos_MAX_y = 730.f;
	const float m_fBGpos_MIN_y = 310.f;
	const float m_fBGscroll_speed = 350.f;
	float m_fBGpos_y;
	float m_fInputDelay;
	const float m_fMAX_DELAY = 0.15f;

	//Probability stuff
	int m_iWeather;
	bool order;

	//Order stuff
	int ingredients;
	int NumOrders;

	//Shop Resources
	float m_fIngredients = 100.f;

	float m_fTrash = 0.f;

	//OrderDelivery variables
	bool m_bOrderArrived = false;
	bool m_bWaitingOrder = false;
	bool m_bDisplayCrate = false;
	bool m_bDisplayTrash = false;
	bool m_bCarryingTrash = false;

	const float m_fMaxDeliveryTimer = 10.f;
	float m_fDeliveryTimeElapsed = 9.f;

	bool m_bDisplay_shop;
	int m_iWorldTime;
	float m_fMinutes;

	vector<DeliveryMan*> m_cDeliveryList;	//List containing all delivery guy(GTH)
	DeliveryMan* deliveryMan;
	Barista* barista;

	StoreMan* storeMan; //Store man handler
	RubbishMan* rubbishMan;

	vector<Customer*> m_cCustomerList;	//List containing all customers
	float m_fCustomerSpawn;	//Spawning timer
	float m_fCustomerRate;	//Rate of spawn

	vector<Vector2> m_v2CustomerWaypointsOUTDOOR;	//List containing all Outdoor waypoints for customer in scene
	vector<Vector2> m_v2CustomerWaypointsINDOOR;	//List containing all Indoor waypoints for customer in scene
	vector<Vector2> m_v2CustomerQueueingPosition;	//List containing all Indoor queueing positions for customer in scene
	vector<Customer*> m_cQueueList;	//List containing queuing customers
};

#endif