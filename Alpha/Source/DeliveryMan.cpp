#include "DeliveryMan.h"


DeliveryMan::DeliveryMan() 
	: m_bTaskFinish(true)
	, currentState(S_IDLE)
	, m_iHoursNeeded(2)
	, m_iResult(0)
	, m_fJourneyTime(0.f)
	, m_fTotalTime(0.f)
{
}

DeliveryMan::~DeliveryMan()
{
}

void DeliveryMan::Update(double dt, int worldTime, int weather, bool order)
{
	//Sleeps for 8 hours before going to Idle(Starts based on the timing that went to sleep)

	//Idle in store until order arrives

	//If need to eat(Lunch, Dinner), finish eating before processing next order
		//Check if 

	//Else process order and start delivering(Delivery speed based on weather and peak period)

	//Return when JourneyTime < 1/2

	//Switches back to idle when JourneyTime == 0

	/*switch (currentState)
	{
	case S_IDLE:
		m_bTaskFinish = true;
		break;
	case S_DELIVERING:
		if (m_fJourneyTime <= m_fTotalTime * 0.5f)
		{
			currentState = S_RETURNING;
		}
		break;
	case S_RETURNING:
		if (m_fJourneyTime <= 0)
		{
			currentState = S_IDLE;
			m_bTaskFinish = true;
		}
		break;
	case S_SLEEPING:
		if (worldTime >= 600)
		{
			currentState = S_IDLE;
			m_bTaskFinish = true;
		}
		break;
	case S_EATING:
		if (worldTime > 1200 || worldTime > 1800)
		{
			currentState = S_IDLE;
			m_bTaskFinish = true;
		}
	default:
		break;
	}

	if (m_bTaskFinish)
	{
		UpdateFSM(worldTime, weather, order);
	}
	
	if (m_fJourneyTime > 0)
	{
		m_fJourneyTime -= (float)dt;
	}*/
	
}

void DeliveryMan::UpdateFSM(int worldTime, int weather, bool order)
{
	//bool Day = false;
	//
	//if (worldTime - 1200 <= 0)
	//{
	//	Day = true;
	//}

	//if (Day)
	//{
	//	if (worldTime < 600)
	//	{
	//		currentState = S_SLEEPING;
	//	}

	//	else if (worldTime == 1200)
	//	{
	//		currentState = S_EATING;
	//	}

	//	/*
	//	Change to something like 
	//	else if( order != NULL)
	//	{
	//		currentState = S_Delivering;
	//		targetPath = order.path;
	//	}
	//	*/

	//	else if (order)
	//	{
	//		currentState = S_DELIVERING;
	//	}
	//}

	//else
	//{
	//	if (worldTime > 2300)
	//	{
	//		currentState = S_SLEEPING;
	//	}

	//	else if (worldTime == 1800)
	//	{
	//		currentState = S_EATING;
	//	}

	//	/*
	//	Change to something like
	//	else if( order != NULL)
	//	{
	//	currentState = S_Delivering;
	//	targetPath = order.path;
	//	}
	//	*/

	//	else if (order)
	//	{
	//		currentState = S_DELIVERING;
	//	}
	//}

	////Generate time
	//if (order)
	//{
	//	if (worldTime > 600 && worldTime < 900 || worldTime > 1700 && worldTime < 2000)
	//	{
	//		m_iHoursNeeded = 2;
	//	}
	//	else
	//	{
	//		m_iHoursNeeded = 1;
	//	}

	//	m_fJourneyTime = weather * m_iHoursNeeded;
	//	m_fTotalTime = m_fJourneyTime;
	//}

	//m_bTaskFinish = false;
}

void DeliveryMan::Draw(SceneManager* sceneManager)
{
	sceneManager->Render2DMesh(this->getMesh(), true, Vector2(1, 1), Vector2(this->getPosition().x, this->getPosition().y), 0);
}

DeliveryMan::STATES DeliveryMan::getCurrentState(void)
{
	return currentState;
}