#ifndef SCENE_MANAGER_CM_PLAY_H
#define SCENE_MANAGER_CM_PLAY_H

#include "SceneManagerGameplay.h"
#include "SceneNode.h"
#include "DeliveryMan.h"
#include "Customer.h"
#include "Application.h"

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
	void InitSprites();
	void InitWayPoints();

	void RenderLight();
	void RenderBG();
	void RenderStaticObject();
	void RenderMobileObject();
	void RenderWaypoints();	//Render waypoints for debug purposes
	void RenderUIInfo();
	void RenderSprites();

	void FetchCustomer();	//Set new customer at spawn
	void UpdateDeliveryMan(double dt);
	void UpdateSprites(double dt);

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

	bool m_bDisplay_shop;
	int m_iWorldTime;
	float m_fMinutes;

	vector<DeliveryMan*> m_cDeliveryList;	//List containing all delivery guy(GTH)
	DeliveryMan* deliveryMan;

	vector<Customer*> m_cCustomerList;	//List containing all customers

	vector<Vector2> m_v2CustomerWaypointsOUTDOOR;	//List containing all Outdoor waypoints for customer in scene
	vector<Vector2> m_v2CustomerWaypointsINDOOR;	//List containing all Indoor waypoints for customer in scene
	vector<Vector2> m_v2CustomerQueueingPosition;	//List containing all Indoor queueing positions for customer in scene
	vector<Customer*> m_cQueueList;	//List containing queuing customers

	SpriteAnimation* Delivery_Out_Up;
	SpriteAnimation* Delivery_Out_Down;
	SpriteAnimation* Delivery_Out_Left;
	SpriteAnimation* Delivery_Out_Right;
	
	SpriteAnimation* Delivery_In_Up;
	SpriteAnimation* Delivery_In_Down;
	SpriteAnimation* Delivery_In_Left;
	SpriteAnimation* Delivery_In_Right;
	
	SpriteAnimation* Barista_Up;
	SpriteAnimation* Barista_Down;
	SpriteAnimation* Barista_Left;
	SpriteAnimation* Barista_Right;
};

#endif