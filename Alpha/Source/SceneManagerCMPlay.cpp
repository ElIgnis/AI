#include "SceneManagerCMPlay.h"

SceneManagerCMPlay::SceneManagerCMPlay()
	: m_iWorldTime(0)
	, m_fMinutes(0.f)
	, m_iWeather(1)
	, order(false)
	, deliveryMan(NULL)
{
}

SceneManagerCMPlay::~SceneManagerCMPlay()
{
}

void SceneManagerCMPlay::Init(const int width, const int height, ResourcePool *RM, InputManager* controls)
{
	SceneManagerGameplay::Init(width, height, RM, controls);

	this->InitShader();

	fpCamera.Init(Vector3(0, 0, -10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	tpCamera.Init(Vector3(0, 0, -10), Vector3(0, 0, 0), Vector3(0, 1, 0));

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 1000 units
	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);

	lightEnabled = true;

	//Sprite Animation vars
	InitSprites();

	//Background variable
	m_fBGpos_y = m_fBGpos_MAX_y;

	//Shop variables
	m_bDisplay_shop = false;

	//Storing waypoints for outdoor customer
	m_v2CustomerWaypointsOUTDOOR.push_back(Vector2(965, 1050));
	m_v2CustomerWaypointsOUTDOOR.push_back(Vector2(965, 700));
	m_v2CustomerWaypointsOUTDOOR.push_back(Vector2(650, 700));
	m_v2CustomerWaypointsOUTDOOR.push_back(Vector2(650, 280));
	m_v2CustomerWaypointsOUTDOOR.push_back(Vector2(1080, 280));
	m_v2CustomerWaypointsOUTDOOR.push_back(Vector2(965, 220));
	m_v2CustomerWaypointsOUTDOOR.push_back(Vector2(965, 20));
	//Storing waypointss for indoor customer
	m_v2CustomerWaypointsINDOOR.push_back(Vector2(1080, 280));
	m_v2CustomerWaypointsINDOOR.push_back(Vector2(1080, 600));
	m_v2CustomerWaypointsINDOOR.push_back(Vector2(800, 500));
	m_v2CustomerWaypointsINDOOR.push_back(Vector2(1000, 600));
	m_v2CustomerWaypointsINDOOR.push_back(Vector2(1080, 280));
	//Starting queue position
	m_v2CustomerQueueingPosition.push_back(Vector2(1080, 600));

	Mesh* drawMesh = resourceManager.retrieveMesh("CUSTOMER");
	drawMesh->textureID = resourceManager.retrieveTexture("CUSTOMER_SPRITE");
	//20 customers in list
	for (unsigned i = 0; i < 20; ++i)
	{
		m_cCustomerList.push_back(new Customer(m_v2CustomerWaypointsOUTDOOR.at(0)));
		m_cCustomerList[i]->setSprite(dynamic_cast<SpriteAnimation*> (drawMesh));
	}

	deliveryMan = new DeliveryMan();
	deliveryMan->Init();
}

void SceneManagerCMPlay::InitSprites()
{
	//Init all the sprites here

	//Outdoors delivery man
	Delivery_Out_Up = (SpriteAnimation*)resourceManager.retrieveMesh("SPRITE_DELIVERY_OUT_UP");
	Delivery_Out_Up->textureID = resourceManager.retrieveTexture("Sprite_Delivery_Out_Up");

	Delivery_Out_Down = (SpriteAnimation*)resourceManager.retrieveMesh("SPRITE_DELIVERY_OUT_DOWN");
	Delivery_Out_Down->textureID = resourceManager.retrieveTexture("Sprite_Delivery_Out_Down");

	Delivery_Out_Left = (SpriteAnimation*)resourceManager.retrieveMesh("SPRITE_DELIVERY_OUT_LEFT");
	Delivery_Out_Left->textureID = resourceManager.retrieveTexture("Sprite_Delivery_Out_Left");

	Delivery_Out_Right = (SpriteAnimation*)resourceManager.retrieveMesh("SPRITE_DELIVERY_OUT_RIGHT");
	Delivery_Out_Right->textureID = resourceManager.retrieveTexture("Sprite_Delivery_Out_Right");

	//Indoors delivery man
	Delivery_In_Up = (SpriteAnimation*)resourceManager.retrieveMesh("SPRITE_DELIVERY_IN_UP");
	Delivery_In_Up->textureID = resourceManager.retrieveTexture("Sprite_Delivery_In_Up");

	Delivery_In_Down = (SpriteAnimation*)resourceManager.retrieveMesh("SPRITE_DELIVERY_IN_DOWN");
	Delivery_In_Down->textureID = resourceManager.retrieveTexture("Sprite_Delivery_In_Down");

	Delivery_In_Left = (SpriteAnimation*)resourceManager.retrieveMesh("SPRITE_DELIVERY_IN_LEFT");
	Delivery_In_Left->textureID = resourceManager.retrieveTexture("Sprite_Delivery_In_Left");

	Delivery_In_Right = (SpriteAnimation*)resourceManager.retrieveMesh("SPRITE_DELIVERY_IN_RIGHT");
	Delivery_In_Right->textureID = resourceManager.retrieveTexture("Sprite_Delivery_In_Right");

	//Barista
	Barista_Up = (SpriteAnimation*)resourceManager.retrieveMesh("SPRITE_BARISTA_UP");
	Barista_Up->textureID = resourceManager.retrieveTexture("Sprite_Barista_Up");

	Barista_Down = (SpriteAnimation*)resourceManager.retrieveMesh("SPRITE_BARISTA_DOWN");
	Barista_Down->textureID = resourceManager.retrieveTexture("Sprite_Barista_Down");

	Barista_Left = (SpriteAnimation*)resourceManager.retrieveMesh("SPRITE_BARISTA_LEFT");
	Barista_Left->textureID = resourceManager.retrieveTexture("Sprite_Barista_Left");

	Barista_Right = (SpriteAnimation*)resourceManager.retrieveMesh("SPRITE_BARISTA_RIGHT");
	Barista_Right->textureID = resourceManager.retrieveTexture("Sprite_Barista_Right");
}

void SceneManagerCMPlay::InitWayPoints()
{
	//Init Indoor waypoints
	//Eating<->Idle 
	//deliveryMan->InitWayPoints_Eat(Vector2());

	//Sleeping<->Idle

	//Idle<->Delivering

	//Path One

	//Path Two

	//Path Three
}

void SceneManagerCMPlay::Update(double dt)
{
	SceneManagerGameplay::Update(dt);

	if (inputManager->getKey("START_DELIVERY") && m_fInputDelay > m_fMAX_DELAY)
	{
		order = true;
		m_fInputDelay = 0.f;
	}
	else
	{
		order = false;
	}

	if (inputManager->getKey("NEW_CUSTOMER") && m_fInputDelay > m_fMAX_DELAY)
	{
		FetchCustomer();
		m_fInputDelay = 0.f;
	}


	if (inputManager->getKey("SHOP_DISPLAY") && m_fInputDelay > m_fMAX_DELAY)
	{
		if (m_bDisplay_shop == false){
			m_bDisplay_shop = true;
		}
		else if (m_bDisplay_shop == true){
			m_bDisplay_shop = false;
		}
		m_fInputDelay = 0.f;
	}

	m_fInputDelay += (float)dt;

	//Increase time based on dt
	m_fMinutes += (float)20 * dt;

	//Updating world time
	if (m_fMinutes > 60){
		m_iWorldTime += 100;
		m_fMinutes = 0;

		//Generates a probability of getting an order when idle every hour
		if (deliveryMan->getCurrentState() == DeliveryMan::S_IDLE)
			order = deliveryMan->GenerateOrder();
	}
		
	//Resetting world time to a new day
	if (m_iWorldTime == 2400){
		m_iWorldTime = 0;
	}
	
	deliveryMan->Update(dt, m_iWorldTime, m_iWeather, order);
	//std::cout << "X: " << Application::getMouse()->getCurrentPosX() << "Y: " << Application::getWindowHeight() - Application::getMouse()->getCurrentPosY() << std::endl;
	//Update customer waypoints
	for (unsigned a = 0; a < m_cCustomerList.size(); ++a)
	{
		//Only update if customer is active
		if (m_cCustomerList[a]->getActive())
		{
			//Update outdoor waypoints
			if (m_cCustomerList[a]->getOutdoorStatus())
			{
				for (unsigned i = 0; i < m_v2CustomerWaypointsOUTDOOR.size(); ++i)
				{
					if (m_v2CustomerWaypointsOUTDOOR[i] == m_cCustomerList[a]->getCurrentPos())
					{
						if (i != (m_v2CustomerWaypointsOUTDOOR.size() - 1))
						{
							m_cCustomerList[a]->setNextPoint(m_v2CustomerWaypointsOUTDOOR[i + 1]);
							break;
						}
						else if (i == (m_v2CustomerWaypointsOUTDOOR.size() - 1))
						{
							m_cCustomerList[a]->setActive(false);
						}
					}
				}
			}
			//Update indoor waypoints
			else
			{
				//Default shop positions
				if (!m_cCustomerList[a]->getQueueStatus())
				{
					for (unsigned i = 0; i < m_v2CustomerWaypointsINDOOR.size(); ++i)
					{
						if (m_v2CustomerWaypointsINDOOR[i] == m_cCustomerList[a]->getCurrentPos())
						{
							if (i != (m_v2CustomerWaypointsINDOOR.size() - 1))
							{
								m_cCustomerList[a]->setNextPoint(m_v2CustomerWaypointsINDOOR[i + 1]);
								break;
							}
						}
					}
				}
				//Queuing shop positions
				else
				{
					//Only if they are not inside the queue we add them into the queue
					if (!m_cCustomerList[a]->getInQueueStatus())
					{
						//Add customer to queue list and setting new waypoint
						m_cCustomerList[a]->setInQueueStatus(true);	//Set the in Queue status to true

						m_cQueueList.push_back(m_cCustomerList[a]);	//Add to vector of queueing customers
						m_cCustomerList[a]->setNextPoint
							(m_v2CustomerQueueingPosition.at(m_v2CustomerQueueingPosition.size() - 1));	//Set the next point to a positions in the queue
						//Generate next position in the queue
						m_v2CustomerQueueingPosition.push_back(Vector2(m_v2CustomerQueueingPosition.at(m_v2CustomerQueueingPosition.size() - 1).x,
							m_v2CustomerQueueingPosition.at(m_v2CustomerQueueingPosition.size() - 1).y - 50));
					}
				}
				//Queueing shop positions
			}
		}
	}
	//Only if there are customers queueing
	if (m_cQueueList.size() > 0)
	{
		//If customer has finished buying, then we remove him
		if (!m_cQueueList[0]->getQueueStatus())
		{
			m_cQueueList[0]->setInQueueStatus(false);	//We set the In queue status to false
			m_cQueueList.erase(m_cQueueList.begin());	//Remove from vector
			m_v2CustomerQueueingPosition.pop_back();	//Remove last queuing position

			//Move all customers forward
			for (unsigned a = 0; a < m_cQueueList.size(); ++a)
			{
				m_cQueueList[a]->setNextPoint(m_v2CustomerQueueingPosition[a]);
			}
		}
	}

	//Update all customers
	for (unsigned i = 0; i < m_cCustomerList.size(); ++i)
	{
		//Only update if active
		if (m_cCustomerList[i]->getActive())
		{
			m_cCustomerList[i]->Update(dt, m_iWorldTime, m_iWeather);
		}
	}

	//Uncomment the following line to play sound
	//resourceManager.retrieveSound("MenuFeedback");
	//tpCamera.UpdatePosition(Vector3(0, 0, 0), Vector3(0, 0, 0));

	fpCamera.Update(dt, 0);
	//tpCamera.Update(dt);

	/*if (inputManager->getKey("BG_DOWN"))
	{
		if (m_fBGpos_y < m_fBGpos_MAX_y)
		{
			m_fBGpos_y += m_fBGscroll_speed * (float)dt;
			if (m_fBGpos_y > m_fBGpos_MAX_y){
				m_fBGpos_y = m_fBGpos_MAX_y;
			}
		}
	}
	if (inputManager->getKey("BG_UP"))
	{
		if (m_fBGpos_y > m_fBGpos_MIN_y)
		{
			m_fBGpos_y -= m_fBGscroll_speed * (float)dt;
			if (m_fBGpos_y < m_fBGpos_MIN_y){
				m_fBGpos_y = m_fBGpos_MIN_y;
			}
		}
	}*/
	UpdateSprites(dt);
}

void SceneManagerCMPlay::UpdateSprites(double dt)
{
	//Update all the sprites here
	Delivery_Out_Up->Update(dt);
	Delivery_Out_Down->Update(dt);
	Delivery_Out_Left->Update(dt);
	Delivery_Out_Right->Update(dt);

	Delivery_In_Up->Update(dt);
	Delivery_In_Down->Update(dt);
	Delivery_In_Left->Update(dt);
	Delivery_In_Right->Update(dt);

	Barista_Up->Update(dt);
	Barista_Down->Update(dt);
	Barista_Left->Update(dt);
	Barista_Right->Update(dt);
}

void SceneManagerCMPlay::UpdateDeliveryMan(double dt)
{

}

void SceneManagerCMPlay::Render()
{
	SceneManagerGameplay::Render();

	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);

	// Set up the view
	viewStack.LoadIdentity();
	viewStack.LookAt(tpCamera.getPosition().x, tpCamera.getPosition().y, tpCamera.getPosition().z,
		tpCamera.getTarget().x, tpCamera.getTarget().y, tpCamera.getTarget().z,
		tpCamera.getUp().x, tpCamera.getUp().y, tpCamera.getUp().z);

	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();

	RenderLight();
	RenderBG();
	RenderWaypoints();
	RenderMobileObject();
	RenderStaticObject();
	RenderUIInfo();
	RenderSprites();
}

void SceneManagerCMPlay::Exit()
{
	while (m_cDeliveryList.size() > 0)
	{
		DeliveryMan *DeliveryMan = m_cDeliveryList.back();
		delete DeliveryMan;
		m_cDeliveryList.pop_back();
	}
	while (m_cCustomerList.size() > 0)
	{
		Customer *customer = m_cCustomerList.back();
		delete customer;
		m_cCustomerList.pop_back();
	}
	if (deliveryMan)
	{
		delete deliveryMan;
		deliveryMan = NULL;
	}
	SceneManagerGameplay::Exit();
}

void SceneManagerCMPlay::BindShaders()
{
	SceneManagerGameplay::BindShaders();
}

// Other specific init, update and render classes
void SceneManagerCMPlay::InitShader()
{
	SHADER thisShader = resourceManager.retrieveShader("Comg");
	programID = LoadShaders(thisShader.vertexShaderDirectory.c_str(), thisShader.fragmentShaderDirectory.c_str());

	this->BindShaders();

	parameters.resize(U_TOTAL);
	lights.resize(1);

	// Get a handle for our uniform
	parameters[U_MVP] = glGetUniformLocation(programID, "MVP");
	//parameters[U_MODEL] = glGetUniformLocation(programID, "M");
	//parameters[U_VIEW] = glGetUniformLocation(programID, "V");
	parameters[U_MODELVIEW] = glGetUniformLocation(programID, "MV");
	parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(programID, "MV_inverse_transpose");
	parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(programID, "material.kAmbient");
	parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(programID, "material.kDiffuse");
	parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(programID, "material.kSpecular");
	parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(programID, "material.kShininess");
	parameters[U_LIGHTENABLED] = glGetUniformLocation(programID, "lightEnabled");
	parameters[U_NUMLIGHTS] = glGetUniformLocation(programID, "numLights");
	parameters[U_LIGHT0_TYPE] = glGetUniformLocation(programID, "lights[0].type");
	parameters[U_LIGHT0_POSITION] = glGetUniformLocation(programID, "lights[0].position_cameraspace");
	parameters[U_LIGHT0_COLOR] = glGetUniformLocation(programID, "lights[0].color");
	parameters[U_LIGHT0_POWER] = glGetUniformLocation(programID, "lights[0].power");
	parameters[U_LIGHT0_KC] = glGetUniformLocation(programID, "lights[0].kC");
	parameters[U_LIGHT0_KL] = glGetUniformLocation(programID, "lights[0].kL");
	parameters[U_LIGHT0_KQ] = glGetUniformLocation(programID, "lights[0].kQ");
	parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(programID, "lights[0].spotDirection");
	parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(programID, "lights[0].cosCutoff");
	parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(programID, "lights[0].cosInner");
	parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(programID, "lights[0].exponent");
	// Get a handle for our "colorTexture" uniform
	parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(programID, "colorTextureEnabled");
	parameters[U_COLOR_TEXTURE] = glGetUniformLocation(programID, "colorTexture");
	// Get a handle for our "textColor" uniform
	parameters[U_TEXT_ENABLED] = glGetUniformLocation(programID, "textEnabled");
	parameters[U_TEXT_COLOR] = glGetUniformLocation(programID, "textColor");

	// Use our shader
	glUseProgram(programID);

	lights[0].type = Light::LIGHT_DIRECTIONAL;
	lights[0].position.Set(0, 0, 10);
	lights[0].color.Set(1, 1, 1);
	lights[0].power = 1;
	lights[0].kC = 1.f;
	lights[0].kL = 0.01f;
	lights[0].kQ = 0.001f;
	lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0].cosInner = cos(Math::DegreeToRadian(30));
	lights[0].exponent = 3.f;
	lights[0].spotDirection.Set(0.f, 0.f, 1.f);

	glUniform1i(parameters[U_NUMLIGHTS], 1);
	glUniform1i(parameters[U_TEXT_ENABLED], 0);

	glUniform1i(parameters[U_LIGHT0_TYPE], lights[0].type);
	glUniform3fv(parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
	glUniform1f(parameters[U_LIGHT0_POWER], lights[0].power);
	glUniform1f(parameters[U_LIGHT0_KC], lights[0].kC);
	glUniform1f(parameters[U_LIGHT0_KL], lights[0].kL);
	glUniform1f(parameters[U_LIGHT0_KQ], lights[0].kQ);
	glUniform1f(parameters[U_LIGHT0_COSCUTOFF], lights[0].cosCutoff);
	glUniform1f(parameters[U_LIGHT0_COSINNER], lights[0].cosInner);
	glUniform1f(parameters[U_LIGHT0_EXPONENT], lights[0].exponent);
}

void SceneManagerCMPlay::InitLight()
{
}

void SceneManagerCMPlay::RenderLight()
{

}

void SceneManagerCMPlay::RenderBG()
{
	//Temp mesh used to draw
	Mesh* drawMesh;

	if (m_bDisplay_shop)
	{
		drawMesh = resourceManager.retrieveMesh("GAME_SHOP");
		drawMesh->textureID = resourceManager.retrieveTexture("GAME_SHOP");
		Render2DMesh(drawMesh, false, Vector2(906, 796), Vector2(900, 650));
	}
	else
	{
		drawMesh = resourceManager.retrieveMesh("GAME_BG");
		drawMesh->textureID = resourceManager.retrieveTexture("GAME_BG");
		Render2DMesh(drawMesh, false, Vector2(1920, 1545), Vector2(960, m_fBGpos_y));
	}
}

void SceneManagerCMPlay::RenderStaticObject()
{
	
}

void SceneManagerCMPlay::RenderMobileObject()
{
	Mesh* drawMesh;

	//switch (m_dmDeliveryGuy->getCurrentState())
	//{
	//case DeliveryMan::S_IDLE:
	//	drawMesh = resourceManager.retrieveMesh("FONT");
	//	drawMesh->textureID = resourceManager.retrieveTexture("AlbaFont");
	//	RenderTextOnScreen(drawMesh, "Idle", resourceManager.retrieveColor("Red"), 75, 400, 550, 0);
	//	break;
	//case DeliveryMan::S_SLEEPING:
	//	drawMesh = resourceManager.retrieveMesh("FONT");
	//	drawMesh->textureID = resourceManager.retrieveTexture("AlbaFont");
	//	RenderTextOnScreen(drawMesh, "Sleeping", resourceManager.retrieveColor("Red"), 75, 400, 550, 0);
	//	break;
	//case DeliveryMan::S_EATING:
	//	drawMesh = resourceManager.retrieveMesh("FONT");
	//	drawMesh->textureID = resourceManager.retrieveTexture("AlbaFont");
	//	RenderTextOnScreen(drawMesh, "Eating", resourceManager.retrieveColor("Red"), 75, 400, 550, 0);
	//	break;
	//case DeliveryMan::S_DELIVERING:
	//	drawMesh = resourceManager.retrieveMesh("FONT");
	//	drawMesh->textureID = resourceManager.retrieveTexture("AlbaFont");
	//	RenderTextOnScreen(drawMesh, "Delivering", resourceManager.retrieveColor("Red"), 75, 400, 550, 0);
	//	break;
	//case DeliveryMan::S_RETURNING:
	//	drawMesh = resourceManager.retrieveMesh("FONT");
	//	drawMesh->textureID = resourceManager.retrieveTexture("AlbaFont");
	//	RenderTextOnScreen(drawMesh, "Returning", resourceManager.retrieveColor("Red"), 75, 400, 550, 0);
	//	break;
	//}

	
	//Render all customers from list
	for (unsigned i = 0; i < m_cCustomerList.size(); ++i)
	{
		//Only render if customers are active
		if (m_cCustomerList[i]->getActive())
		{
			//Render indoor customers
			if (m_bDisplay_shop)
			{
				if (m_cCustomerList[i]->getOutdoorStatus() == false)
				{
					Render2DMesh(m_cCustomerList[i]->getSprite() , false, Vector2(50, 50), m_cCustomerList[i]->getCurrentPos());
				}
			}
			//Render outdoor customers
			else
			{
				if (m_cCustomerList[i]->getOutdoorStatus())
				{
					Render2DMesh(m_cCustomerList[i]->getSprite(), false, Vector2(50, 50), m_cCustomerList[i]->getCurrentPos());
				}
			}
		}
	}
}

void SceneManagerCMPlay::RenderWaypoints()
{
	Mesh* drawMesh = resourceManager.retrieveMesh("CUSTOMER_WAYPOINT");
	drawMesh->textureID = resourceManager.retrieveTexture("GRASS");
	//Rendering of indoor waypoints
	if (m_bDisplay_shop)
	{
		for (unsigned i = 0; i < m_v2CustomerWaypointsINDOOR.size(); ++i)
		{
			Render2DMesh(drawMesh, false, Vector2(50, 50), m_v2CustomerWaypointsINDOOR[i]);
		}
	}
	//Rendering of outdoor waypoints
	else
	{
		for (unsigned i = 0; i < m_v2CustomerWaypointsOUTDOOR.size(); ++i)
		{
			Render2DMesh(drawMesh, false, Vector2(50, 50), m_v2CustomerWaypointsOUTDOOR[i]);
		}
	}
}

void SceneManagerCMPlay::RenderUIInfo()
{
	//Display Weather and time here
	Mesh* drawMesh;

	drawMesh = resourceManager.retrieveMesh("FONT");
	drawMesh->textureID = resourceManager.retrieveTexture("Font");
	RenderTextOnScreen(drawMesh, "Current Time: " + std::to_string(m_iWorldTime), resourceManager.retrieveColor("Red"), 75, sceneWidth - 500, sceneHeight - 100, 0);
	RenderTextOnScreen(drawMesh, std::to_string(deliveryMan->getCurrentState()), resourceManager.retrieveColor("Red"), 75, sceneWidth - 500, sceneHeight - 200, 0);
}

void SceneManagerCMPlay::RenderSprites()
{
	//Render all the sprites here

	//Indoor deliveryman
	if (m_bDisplay_shop)
	{
		if (!deliveryMan->getOutdoor())
		Render2DMesh(Delivery_In_Up, false, Vector2(50, 50), deliveryMan->GetPos());
	}

	//Outdoor deliveryman
	else
	{
		if (deliveryMan->getOutdoor())
			Render2DMesh(Delivery_Out_Up, false, Vector2(50, 100), deliveryMan->GetPos());
	}
	std::cout << deliveryMan->m_bPendingDelivery << std::endl;
	//Render2DMesh(Delivery_Out_Left, false, Vector2(100, 50), Vector2(600, 700));
	//Render2DMesh(Delivery_Out_Up, false, Vector2(50, 100), Vector2(650, 600));
}


void SceneManagerCMPlay::FetchCustomer()
{
	for (unsigned i = 0; i < m_cCustomerList.size(); ++i)
	{
		if (m_cCustomerList[i]->getActive() == false)
		{
			m_cCustomerList[i]->setActive(true);
			m_cCustomerList[i]->setStartPos(m_v2CustomerWaypointsOUTDOOR.at(0));
			break;
		}
	}
}