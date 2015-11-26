#ifndef SCENE_MANAGER_CM_PLAY_H
#define SCENE_MANAGER_CM_PLAY_H

#include "SceneManagerGameplay.h"
#include "SceneNode.h"
#include "DeliveryMan.h"

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

private:
	const float m_fBGpos_MAX_y = 730.f;
	const float m_fBGpos_MIN_y = 310.f;
	const float m_fBGscroll_speed = 350.f;
	float m_fBGpos_y;

	//Probability stuff
	int m_iWeather;
	bool order;

	bool m_bDisplay_shop;
	int m_iWorldTime;
	float m_fMinutes;
	DeliveryMan* m_dmDeliveryGuy;
};

#endif