#include "SceneManagerCMPlay.h"

SceneManagerCMPlay::SceneManagerCMPlay()
	: m_iWorldTime(800)
	, m_fMinutes(0.f)
	, m_iWeather(1)
	, order(false)
	, NumOrders(0)
	, NumDeliveryOrders(0)
	, deliveryMan(NULL)
	, barista(NULL)
	, m_fCustomerSpawn(0.f)
	, m_fCustomerRate(1.f)
	, CustomerID(0)
	, CuttingQueue(false)
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
	m_v2CustomerWaypointsINDOOR.push_back(Vector2(1150, 600));
	m_v2CustomerWaypointsINDOOR.push_back(Vector2(900, 550));
	m_v2CustomerWaypointsINDOOR.push_back(Vector2(1000, 600));
	m_v2CustomerWaypointsINDOOR.push_back(Vector2(1080, 280));
	//Starting queue position
	m_v2CustomerQueueingPosition.push_back(Vector2(1150, 600));
	//Starting wait position
	m_v2CustomerWaitingPosition.push_back(Vector2(900, 550));

	Mesh* drawMesh = resourceManager.retrieveMesh("CUSTOMER");
	drawMesh->textureID = resourceManager.retrieveTexture("CUSTOMER_SPRITE");
	//20 customers in list
	for (unsigned i = 0; i < 20; ++i)
	{
		Customer* temp = new Customer(m_v2CustomerWaypointsOUTDOOR.at(0));
		temp->setSprite(dynamic_cast<SpriteAnimation*> (drawMesh));
		m_cCustomerList.push_back(temp);
	}

	deliveryMan = new DeliveryMan();
	deliveryMan->Init();

	drawMesh = resourceManager.retrieveMesh("SPRITE_DELIVERY_IN");
	drawMesh->textureID = resourceManager.retrieveTexture("Sprite_Delivery_In");
	deliveryMan->SetIndoorSpriteAnim(dynamic_cast<SpriteAnimation*> (drawMesh));

	drawMesh = resourceManager.retrieveMesh("SPRITE_DELIVERY_OUT");
	drawMesh->textureID = resourceManager.retrieveTexture("Sprite_Delivery_Out");
	deliveryMan->SetOutdoorSpriteAnim(dynamic_cast<SpriteAnimation*> (drawMesh));

	drawMesh = resourceManager.retrieveMesh("SPRITE_DELIVERY_OUT_NIGHT");
	drawMesh->textureID = resourceManager.retrieveTexture("Sprite_Delivery_Out_Night");
	deliveryMan->SetOutdoorSpriteAnim_Night(dynamic_cast<SpriteAnimation*> (drawMesh));

	barista = new Barista();
	barista->Init();

	drawMesh = resourceManager.retrieveMesh("SPRITE_BARISTA");
	drawMesh->textureID = resourceManager.retrieveTexture("Sprite_Barista");
	barista->SetSpriteAnim(dynamic_cast<SpriteAnimation*> (drawMesh));

	storeMan = new StoreMan();
	storeMan->Init();

	drawMesh = resourceManager.retrieveMesh("STORE_MAN");
	drawMesh->textureID = resourceManager.retrieveTexture("Sprite_StoreMan");
	storeMan->SetSpriteAnim(dynamic_cast<SpriteAnimation*>(drawMesh));

	rubbishMan = new RubbishMan();
	rubbishMan->Init();

	drawMesh = resourceManager.retrieveMesh("RUBBISH_MAN");
	drawMesh->textureID = resourceManager.retrieveTexture("Sprite_RubbishMan");
	rubbishMan->SetSpriteAnim(dynamic_cast<SpriteAnimation*>(drawMesh));

	shop_mb = new MessageBoard;
	customer_mb = new MessageBoard;
}

void SceneManagerCMPlay::Update(double dt)
{
	SceneManagerGameplay::Update(dt);

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
	if (inputManager->getKey("CHANGE_INGREDIENT") && m_fInputDelay > m_fMAX_DELAY)
	{
		m_fReserve = 0.f;
		m_fInputDelay = 0.f;
		m_fTrash = 100.0f;
	}

	if (m_iWorldTime >= 0 && m_iWorldTime <= 700
		|| m_iWorldTime >= 1900)
	{
		//Do nothing if night time
	}
	else
	{
		//Spawn customer
		if (m_fCustomerSpawn >= m_fCustomerRate)
		{
			//FetchCustomer();
			m_fCustomerSpawn = 0.f;
			m_fCustomerRate = Math::RandFloatMinMax(0.3f, 1.5f);
		}
	}
	m_fCustomerSpawn += (float)dt;
	m_fInputDelay += (float)dt;

	//Increase time based on dt
	m_fMinutes += (float)20 * dt;

	//Updating world time
	if (m_fMinutes > 60){
		m_iWorldTime += 100;
		m_fMinutes = 0;

		//Generates a probability of getting an order when idle every hour
		if (deliveryMan->getCurrentState() == DeliveryMan::S_IDLE)
		{
			order = deliveryMan->GenerateOrder();
			if (order)
			{
				++NumDeliveryOrders;
				barista->addNumDeliveryOrders(1);
			}
		}
	}
		
	//Resetting world time to a new day
	if (m_iWorldTime == 2400){
		m_iWorldTime = 0;
	}
	
	deliveryMan->Update(dt, m_iWorldTime, m_iWeather, order, shop_mb);

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
				//If they are not queuing we update the normal waypoints
				if (!m_cCustomerList[a]->getQueueStatus() && !m_cCustomerList[a]->getWaitStatus())
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
				else if (m_cCustomerList[a]->getQueueStatus() && !m_cCustomerList[a]->getWaitStatus())
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
				//Waiting shop positions
				else if (m_cCustomerList[a]->getWaitStatus())
				{
					//Only if they are not inside the wait list we add them into the list
					if (!m_cCustomerList[a]->getInWaitStatus())
					{
						//Add customer to wait list and setting new waypoint
						m_cCustomerList[a]->setInWaitStatus(true);	//Set the in wait status to true

						m_cWaitList.push_back(m_cCustomerList[a]);	//Add to vector of waiting customers
						m_cCustomerList[a]->setNextPoint
							(m_v2CustomerWaitingPosition.at(m_v2CustomerWaitingPosition.size() - 1));	//Set the next point to a positions in the queue
						//Generate next position in the queue
						m_v2CustomerWaitingPosition.push_back(Vector2(m_v2CustomerWaitingPosition.at(m_v2CustomerWaitingPosition.size() - 1).x,
							m_v2CustomerWaitingPosition.at(m_v2CustomerWaitingPosition.size() - 1).y - 50));
					}
				}
			}
		}
	}
	//Only if there are customers queueing
	if (m_cQueueList.size() > 0)
	{
		for (unsigned a = 0; a < m_cQueueList.size(); ++a)
		{
			if (m_cQueueList[a]->getCutQueueStatus())
			{
				CuttingQueue = true;
				break;
			}
			else if (a = m_cQueueList.size() - 1)
			{
				CuttingQueue = false;
			}
		}
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

		if (m_cQueueList.size() > 0)
		{
			//We start from the customer behind
			for (unsigned i = m_cQueueList.size() - 1; i > 0; --i)
			{
				int temp = i - 1;
				//Only if there is a customer infront of the current customer
				if (temp >= 0)
				{
					//Only if the customer infront of the current customer is queueing and is not the one that has already cut queue
					if (m_cQueueList[i - 1]->getState() == Customer::S_QUEUE && !m_cQueueList[i - 1]->getCutQueueStatus())
					{
						m_cQueueList[i]->setCutQueueStatus(true, m_cQueueList[i - 1]->GetQueueID());
						break;
					}
				}
			}

			for (unsigned i = m_cQueueList.size() - 1; i > 0; --i)
			{
				if (customer_mb->GetMsg(Urgent, std::to_string(m_cQueueList[i]->GetQueueID())))
				{
					m_cQueueList[i + 1]->setNextPoint(m_cQueueList[i]->getNextPoint());	//Set the customer behind to cut the customer infront
					m_cQueueList[i]->setNextPoint(m_cQueueList[i]->getCurrentPos());	//Set customer infront to stay at his own position
					/*Vector2 temp = m_v2CustomerQueueingPosition[i + 1];
					m_v2CustomerQueueingPosition[i + 1] = m_v2CustomerQueueingPosition[i];
					m_v2CustomerQueueingPosition[i] = temp;*/
					Customer* temp = m_cQueueList[i + 1];//Swapping them
					m_cQueueList[i + 1] = m_cQueueList[i];	
					m_cQueueList[i] = temp;
					//m_cQueueList[i + 1]->setNextPoint(Temp);	//Moving the one in front backwards
					//m_cQueueList[i]->setNextPoint(Temp2);	//
				}
			}
		}
	}
	//Update all customers
	for (unsigned i = 0; i < m_cCustomerList.size(); ++i)
	{
		//Only update if active
		if (m_cCustomerList[i]->getActive())
		{
			//Update number of orders based on orders placed
			if (m_cCustomerList[i]->getOrderPlaced())
			{
				barista->addNumOrders(1);
				m_cCustomerList[i]->setOrderPlaced(false);
			}
			else if (m_cCustomerList[i]->getState() == Customer::S_WAIT && !m_cCustomerList[i]->getWaitStatus())
			{
				//Only if drinks are available to pick up
				if (barista->GetDrinkPrepared())
				{
					NumOrders++;
					m_cCustomerList[i]->setDrinkAvailable(true);
					barista->SubtractDrinkPrepared();
					//Only if there are customers waiting
					if (m_cWaitList.size() > 0)
					{
						//If customer has finished waiting, then we remove him
						if (!m_cWaitList[0]->getWaitStatus())
						{
							m_cWaitList[0]->setInWaitStatus(false);	//We set the In wait status to false
							m_cWaitList.erase(m_cWaitList.begin());	//Remove from vector
							m_v2CustomerWaitingPosition.pop_back();	//Remove last waiting position

							//Move all customers forward
							for (unsigned a = 0; a < m_cWaitList.size(); ++a)
							{
								m_cWaitList[a]->setNextPoint(m_v2CustomerWaitingPosition[a]);
							}
						}
					}
				}
			}
			else if (m_cCustomerList[i]->getState() == Customer::S_PICKUP)
			{
				if (m_cCustomerList[i]->getDelay() > 0.1f)
				{
					m_cCustomerList[i]->setPickedUp(true);
					NumOrders--;
					Mesh* drawMesh = resourceManager.retrieveMesh("CUSTOMER2");
					drawMesh->textureID = resourceManager.retrieveTexture("CUSTOMER_SPRITE2");
					m_cCustomerList[i]->setSprite(dynamic_cast<SpriteAnimation*> (drawMesh));
				}
			}

			m_cCustomerList[i]->Update(dt, m_iWorldTime, m_iWeather, customer_mb);
		}
	}

	barista->Update(dt, m_fIngredients, m_fTrash, m_fReserve, shop_mb);

	storeMan->Update(dt, &m_fReserve, &m_bOrderArrived, &m_bWaitingOrder);

	if (m_bWaitingOrder)
	{
		UpdateGoodsDelivery(dt);
	}

	if (m_bDisplayCrate)
	{
		if (storeMan->GetPosition() == Vector2(500, 850))
		{
			m_bDisplayCrate = false;
		}
	}

	rubbishMan->Update(dt, m_iWorldTime, &m_fTrash);

	if (m_fTrash >= 80 && !m_bCarryingTrash)
	{
		m_bDisplayTrash = true;
	}
	else if (m_fTrash >= 80 && m_bCarryingTrash)
	{
		m_bDisplayTrash = false;
	}
	
	if (m_fTrash == 0)
	{
		m_bCarryingTrash = false;
	}

	if (m_bDisplayTrash)
	{
		if (rubbishMan->GetPosition() == Vector2(545, 725))
		{
			m_bCarryingTrash = true;
		}
	}

	fpCamera.Update(dt, 0);
}

void SceneManagerCMPlay::UpdateGoodsDelivery(double dt)
{
	//If waiting for order update waiting
	m_fDeliveryTimeElapsed += dt;

	if (m_fDeliveryTimeElapsed >= m_fMaxDeliveryTimer)
	{
		m_bOrderArrived = true;
		m_bDisplayCrate = true;
		m_fDeliveryTimeElapsed = 0.f;
	}
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
	RenderStaticObject();
	RenderMobileObject();
	RenderUIInfo();
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
	if (storeMan)
	{
		delete storeMan;
		storeMan = NULL;
	}
	if (rubbishMan)
	{
		delete rubbishMan;
		rubbishMan = NULL;
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
		Render2DMesh(drawMesh, false, Vector2(1246, 800), Vector2(730, 650));
	}
	else
	{
		if (m_iWorldTime >= 0 && m_iWorldTime <= 700
			|| m_iWorldTime >= 1900)
		{
			drawMesh = resourceManager.retrieveMesh("GAME_BG_NIGHT");
			drawMesh->textureID = resourceManager.retrieveTexture("GAME_BG_NIGHT");
			Render2DMesh(drawMesh, false, Vector2(1920, 1545), Vector2(960, m_fBGpos_y));
		}
		else
		{
			drawMesh = resourceManager.retrieveMesh("GAME_BG");
			drawMesh->textureID = resourceManager.retrieveTexture("GAME_BG");
			Render2DMesh(drawMesh, false, Vector2(1920, 1545), Vector2(960, m_fBGpos_y));
		}
	}
}

void SceneManagerCMPlay::RenderStaticObject()
{	
	//Temp mesh used to draw
	Mesh* drawMesh;
	
	if (m_bDisplay_shop)
	{
		drawMesh = resourceManager.retrieveMesh("GAME_TELEPHONE");
		drawMesh->textureID = resourceManager.retrieveTexture("GAME_TELEPHONE");
		Render2DMesh(drawMesh, false, Vector2(25, 25), Vector2(700, 725));

		drawMesh = resourceManager.retrieveMesh("GAME_RUBBISHBIN");
		drawMesh->textureID = resourceManager.retrieveTexture("GAME_RUBBISHBIN");
		Render2DMesh(drawMesh, false, Vector2(40, 40), Vector2(710, 950));

		drawMesh = resourceManager.retrieveMesh("HORSEFLIP");
		drawMesh->textureID = resourceManager.retrieveTexture("HorseFlip");
		Render2DMesh(drawMesh, false, Vector2(200, 100), Vector2(225, 900));
		if (NumOrders > 0)
		{
			drawMesh = resourceManager.retrieveMesh("DRINKS");
			drawMesh->textureID = resourceManager.retrieveTexture("Drinks");
			Render2DMesh(drawMesh, false, Vector2(50, 50), Vector2(1000, 660));
		}
		if (barista->getNumDeliveryPrepared() > 0)
		{
			drawMesh = resourceManager.retrieveMesh("DRINKS");
			drawMesh->textureID = resourceManager.retrieveTexture("Drinks");
			Render2DMesh(drawMesh, false, Vector2(50, 50), Vector2(900, 660));
		}
		if (m_fReserve >= 20)
		{
			drawMesh = resourceManager.retrieveMesh("GAME_CRATE");
			drawMesh->textureID = resourceManager.retrieveTexture("GAME_CRATE");
			Render2DMesh(drawMesh, false, Vector2(40, 40), Vector2(825, 650));
		}

		if (m_bDisplayTrash)
		{
			drawMesh = resourceManager.retrieveMesh("GAME_RUBBISHBAG");
			drawMesh->textureID = resourceManager.retrieveTexture("GAME_RUBBISHBAG");
			Render2DMesh(drawMesh, false, Vector2(40, 40), Vector2(515, 725));
		}
		if (m_bDisplayCrate)
		{
			drawMesh = resourceManager.retrieveMesh("GAME_CRATE");
			drawMesh->textureID = resourceManager.retrieveTexture("GAME_CRATE");
			Render2DMesh(drawMesh, false, Vector2(40, 40), Vector2(450, 850));
		}
		if (barista->getCurrentState() == Barista::S_BREW)
		{
			drawMesh = resourceManager.retrieveMesh("DRINKS_2");
			drawMesh->textureID = resourceManager.retrieveTexture("Drinks_2");
			Render2DMesh(drawMesh, false, Vector2(40, 40), Vector2(1050, 660));
			Render2DMesh(drawMesh, false, Vector2(40, 40), Vector2(1100, 660));
		}
		if (deliveryMan->getCurrentState() == DeliveryMan::S_EATING)
		{
			drawMesh = resourceManager.retrieveMesh("FOOD");
			drawMesh->textureID = resourceManager.retrieveTexture("Food");
			Render2DMesh(drawMesh, false, Vector2(40, 40), Vector2(670, 690));
		}
	}
}

void SceneManagerCMPlay::RenderMobileObject()
{
	Mesh* drawMesh;

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
					if (m_cCustomerList[i]->getCurrentState() == Customer::S_DECIDE)
					{
						drawMesh = resourceManager.retrieveMesh("QUESTION_MARK");
						drawMesh->textureID = resourceManager.retrieveTexture("Question_mark");
						Render2DMesh(drawMesh, false, Vector2(50, 50), Vector2(m_cCustomerList[i]->getCurrentPos().x, m_cCustomerList[i]->getCurrentPos().y + 50.f));
					}
				}
			}
		}
	}

	//Render StoreMan
	if (m_bDisplay_shop)
	{
		storeMan->Draw(this);
		rubbishMan->Draw(this);
	}

	drawMesh = resourceManager.retrieveMesh("HORSE");
	drawMesh->textureID = resourceManager.retrieveTexture("Horse");

	//Indoor deliveryman
	if (m_bDisplay_shop)
	{
		if (!deliveryMan->getOutdoor())
		{
			if (!deliveryMan->getInCarriage())
			{
				Render2DMesh(deliveryMan->GetIndoorSpriteAnim(), false, Vector2(50, 50), deliveryMan->GetPos());
				Render2DMesh(drawMesh, false, Vector2(200, 100), Vector2(850, 895));
			}
			else
				Render2DMesh(deliveryMan->GetOutdoorSpriteAnim(), false, Vector2(175, 175), deliveryMan->GetPos());
		}
		Render2DMesh(barista->GetSpriteAnim(), false, Vector2(50, 50), barista->GetPos());
	}

	//Outdoor deliveryman
	else
	{
		if (deliveryMan->getOutdoor())
		{
			//Night time sprite
			if (m_iWorldTime >= 0 && m_iWorldTime <= 0700
				|| m_iWorldTime >= 1900)
			{
				Render2DMesh(deliveryMan->GetOutdoorSpriteAnim_Night(), false, Vector2(100, 100), deliveryMan->GetPos());
			}
			//Day time sprite
			else
			{
				Render2DMesh(deliveryMan->GetOutdoorSpriteAnim(), false, Vector2(100, 100), deliveryMan->GetPos());
			}
		}
	}
}

void SceneManagerCMPlay::RenderWaypoints()
{
}

void SceneManagerCMPlay::RenderUIInfo()
{
	//Display Weather and time here
	Mesh* drawMesh;

	drawMesh = resourceManager.retrieveMesh("BLACK_QUAD");
	drawMesh->textureID = resourceManager.retrieveTexture("Black_Quad");
	Render2DMesh(drawMesh, false, Vector2(425, 80), Vector2(sceneWidth - 330, sceneHeight - 100));
	
	drawMesh = resourceManager.retrieveMesh("FONT");
	drawMesh->textureID = resourceManager.retrieveTexture("Font");
	RenderTextOnScreen(drawMesh, "Current Time: " + std::to_string(m_iWorldTime), resourceManager.retrieveColor("Red"), 75, sceneWidth - 500, sceneHeight - 100, 0);
	
	if (m_bDisplay_shop)
	{
		RenderTextOnScreen(drawMesh, "Reserve: " + std::to_string(m_fReserve), resourceManager.retrieveColor("Red"), 75, sceneWidth - 500, sceneHeight - 200, 0);
		RenderTextOnScreen(drawMesh, "Ingredients: " + std::to_string(m_fIngredients), resourceManager.retrieveColor("Red"), 75, sceneWidth - 500, sceneHeight - 300, 0);
		RenderTextOnScreen(drawMesh, "Number of orders: " + std::to_string(barista->getNumOrders()), resourceManager.retrieveColor("Red"), 75, sceneWidth - 500, sceneHeight - 400, 0);
		RenderTextOnScreen(drawMesh, "Number of deliveries: " + std::to_string(NumDeliveryOrders), resourceManager.retrieveColor("Red"), 75, sceneWidth - 500, sceneHeight - 500, 0);
		RenderTextOnScreen(drawMesh, "Trash: " + std::to_string(m_fTrash), resourceManager.retrieveColor("Red"), 75, sceneWidth - 500, sceneHeight - 600, 0);

		RenderTextOnScreen(drawMesh, "Shop Message Board:", resourceManager.retrieveColor("Red"), 30, sceneWidth - 500, (sceneHeight - 670), 0);
		//Render messages from message board
		for (int i = 0; i < shop_mb->GetDisplayBoard().size(); ++i)
		{
			RenderTextOnScreen(drawMesh, shop_mb->GetDisplayBoard().at(i), resourceManager.retrieveColor("Red"), 30, sceneWidth - 500, (sceneHeight - 700 - 40 * i), 0);
		}

		RenderTextOnScreen(drawMesh, "Customer Message Board:", resourceManager.retrieveColor("Red"), 30, 100, (sceneHeight - 670), 0);
		//Render messages from message board
		for (int i = 0; i < customer_mb->GetDisplayBoard().size(); ++i)
		{
			RenderTextOnScreen(drawMesh, customer_mb->GetDisplayBoard().at(i), resourceManager.retrieveColor("Red"), 30, 100, (sceneHeight - 700 - 40 * i), 0);
		}
	}
	switch (storeMan->getCurrentState())
	{
	case StoreMan::S_IDLE:
		if (m_bDisplay_shop)
			RenderTextOnScreen(drawMesh, "Idle", resourceManager.retrieveColor("Red"), 40, storeMan->GetPosition().x, storeMan->GetPosition().y + 50, 0);
		break;
	case StoreMan::S_NEWORDER:
		if (m_bDisplay_shop)
			RenderTextOnScreen(drawMesh, "Make Order", resourceManager.retrieveColor("Red"), 40, storeMan->GetPosition().x - 30, storeMan->GetPosition().y + 50, 0);
		break;
	case StoreMan::S_MOVEORDER:
		if (m_bDisplay_shop)
			RenderTextOnScreen(drawMesh, "Move Order", resourceManager.retrieveColor("Red"), 40, storeMan->GetPosition().x - 30, storeMan->GetPosition().y + 50, 0);
		break;
	default:
		break;
	}

	switch (rubbishMan->getCurrentState())
	{
	case RubbishMan::S_IDLE:
		if (m_bDisplay_shop)
			RenderTextOnScreen(drawMesh, "Idle", resourceManager.retrieveColor("Red"), 40, rubbishMan->GetPosition().x, rubbishMan->GetPosition().y + 50, 0);
		break;
	case RubbishMan::S_EAT:
		if (m_bDisplay_shop)
			RenderTextOnScreen(drawMesh, "Eat", resourceManager.retrieveColor("Red"), 40, rubbishMan->GetPosition().x, rubbishMan->GetPosition().y + 50, 0);
		break;
	case RubbishMan::S_TAKETRASH:
		if (m_bDisplay_shop)
			RenderTextOnScreen(drawMesh, "Take Trash", resourceManager.retrieveColor("Red"), 40, rubbishMan->GetPosition().x - 30, rubbishMan->GetPosition().y + 50, 0);
		break;
	default:
		break;
	}

	switch (deliveryMan->getCurrentState())
	{
	case DeliveryMan::S_IDLE:
		if (m_bDisplay_shop)
			RenderTextOnScreen(drawMesh, "Idle", resourceManager.retrieveColor("Red"), 40, deliveryMan->GetPos().x, deliveryMan->GetPos().y + 50, 0);
		break;
	case DeliveryMan::S_SLEEPING:
		if (m_bDisplay_shop)
			RenderTextOnScreen(drawMesh, "Sleeping", resourceManager.retrieveColor("Red"), 40, deliveryMan->GetPos().x - 15, deliveryMan->GetPos().y + 50, 0);
		break;
	case DeliveryMan::S_COLLECTING:
		if (m_bDisplay_shop)
			RenderTextOnScreen(drawMesh, "Collecting", resourceManager.retrieveColor("Red"), 40, deliveryMan->GetPos().x - 15, deliveryMan->GetPos().y + 50, 0);
		break;
	case DeliveryMan::S_EATING:
		if (m_bDisplay_shop)
			RenderTextOnScreen(drawMesh, "Eating", resourceManager.retrieveColor("Red"), 40, deliveryMan->GetPos().x - 15, deliveryMan->GetPos().y + 50, 0);
		break;
	case DeliveryMan::S_DELIVERING:
		if (!m_bDisplay_shop)
			RenderTextOnScreen(drawMesh, "Delivering", resourceManager.retrieveColor("Red"), 40, deliveryMan->GetPos().x - 15, deliveryMan->GetPos().y + 50, 0);
		break;
	case DeliveryMan::S_RETURNING:
		if (!m_bDisplay_shop && deliveryMan->getOutdoor() || m_bDisplay_shop && !deliveryMan->getOutdoor())
			RenderTextOnScreen(drawMesh, "Returning", resourceManager.retrieveColor("Red"), 40, deliveryMan->GetPos().x - 15, deliveryMan->GetPos().y + 50, 0);
		break;
	default:
		break;
	}

	switch (barista->getCurrentState())
	{
	case Barista::S_IDLE:
		if (m_bDisplay_shop)
			RenderTextOnScreen(drawMesh, "Idle", resourceManager.retrieveColor("Red"), 40, barista->GetPos().x, barista->GetPos().y + 50, 0);
		break;
	case Barista::S_BREW:
		if (m_bDisplay_shop)
			RenderTextOnScreen(drawMesh, "Brewing", resourceManager.retrieveColor("Red"), 40, barista->GetPos().x - 15, barista->GetPos().y + 50, 0);
		break;
	case Barista::S_REFILL:
		if (m_bDisplay_shop)
			RenderTextOnScreen(drawMesh, "Refilling", resourceManager.retrieveColor("Red"), 40, barista->GetPos().x - 15, barista->GetPos().y + 50, 0);
		break;
	default:
		break;
	}

	//Customer
	for (unsigned i = 0; i < m_cCustomerList.size(); i++)
	{
		if (m_cCustomerList[i]->getActive())
		{

			if (!m_cCustomerList[i]->getOutdoorStatus())
			{
				switch (m_cCustomerList[i]->getCurrentState())
				{
				case Customer::S_IDLE:
					if (m_bDisplay_shop)
						RenderTextOnScreen(drawMesh, "Idle", resourceManager.retrieveColor("Red"), 40, m_cCustomerList[i]->getPos().x, m_cCustomerList[i]->getPos().y + 50, 0);
					break;
				case Customer::S_WALKING:
					if (m_bDisplay_shop)
						RenderTextOnScreen(drawMesh, "Walking", resourceManager.retrieveColor("Red"), 40, m_cCustomerList[i]->getPos().x, m_cCustomerList[i]->getPos().y + 50, 0);
					break;
				case Customer::S_WAIT:
					if (m_bDisplay_shop)
						RenderTextOnScreen(drawMesh, "Waiting", resourceManager.retrieveColor("Red"), 40, m_cCustomerList[i]->getPos().x, m_cCustomerList[i]->getPos().y + 50, 0);
					break;
				case Customer::S_PICKUP:
					if (m_bDisplay_shop)
						RenderTextOnScreen(drawMesh, "Pick up", resourceManager.retrieveColor("Red"), 40, m_cCustomerList[i]->getPos().x, m_cCustomerList[i]->getPos().y + 50, 0);
					break;
				case Customer::S_QUEUE:
					if (m_bDisplay_shop)
						RenderTextOnScreen(drawMesh, "Queue", resourceManager.retrieveColor("Red"), 40, m_cCustomerList[i]->getPos().x, m_cCustomerList[i]->getPos().y + 50, 0);
					break;
				case Customer::S_BUY:
					if (m_bDisplay_shop)
						RenderTextOnScreen(drawMesh, "Ordering", resourceManager.retrieveColor("Red"), 40, m_cCustomerList[i]->getPos().x, m_cCustomerList[i]->getPos().y + 50, 0);
					break;
				default:
					break;
				}

				RenderTextOnScreen(drawMesh, std::to_string(m_cCustomerList[i]->GetQueueID()), resourceManager.retrieveColor("Red"), 40, m_cCustomerList[i]->getPos().x + 90, m_cCustomerList[i]->getPos().y + 50, 0);
			}
			else if (!m_bDisplay_shop)
			{
				switch (m_cCustomerList[i]->getCurrentState())
				{
				case Customer::S_IDLE:
					RenderTextOnScreen(drawMesh, "Idle", resourceManager.retrieveColor("Red"), 40, m_cCustomerList[i]->getPos().x, m_cCustomerList[i]->getPos().y + 50, 0);
					break;
				case Customer::S_WALKING:
					RenderTextOnScreen(drawMesh, "Walking", resourceManager.retrieveColor("Red"), 40, m_cCustomerList[i]->getPos().x, m_cCustomerList[i]->getPos().y + 50, 0);
					break;
				case Customer::S_DECIDE:
					RenderTextOnScreen(drawMesh, "Deciding", resourceManager.retrieveColor("Red"), 40, m_cCustomerList[i]->getPos().x, m_cCustomerList[i]->getPos().y + 50, 0);
					break;
				default:
					break;
				}

				//RenderTextOnScreen(drawMesh, std::to_string(m_cCustomerList[i]->GetQueueID()), resourceManager.retrieveColor("Red"), 40, m_cCustomerList[i]->getPos().x, m_cCustomerList[i]->getPos().y + 90, 0);
			}
		}
	}
}

void SceneManagerCMPlay::FetchCustomer()
{
	Mesh* drawMesh = resourceManager.retrieveMesh("CUSTOMER");
	drawMesh->textureID = resourceManager.retrieveTexture("CUSTOMER_SPRITE");
	CustomerID++;
	for (unsigned i = 0; i < m_cCustomerList.size(); ++i)
	{
		if (m_cCustomerList[i]->getActive() == false)
		{
			m_cCustomerList[i]->Reset();
			m_cCustomerList[i]->SetQueueID(CustomerID);
			m_cCustomerList[i]->setActive(true);
			m_cCustomerList[i]->setStartPos(m_v2CustomerWaypointsOUTDOOR.at(0));
			m_cCustomerList[i]->setSprite(dynamic_cast<SpriteAnimation*> (drawMesh));
			break;
		}
	}
}