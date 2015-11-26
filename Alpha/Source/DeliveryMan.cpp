#include "DeliveryMan.h"


DeliveryMan::DeliveryMan() : taskFinish(true), currentState(S_IDLE)
{
}


DeliveryMan::~DeliveryMan()
{
}

void DeliveryMan::Update(double dt, int worldTime, bool order)
{
	if (taskFinish)
	{
		UpdateFSM(worldTime, order);
	}
}

void DeliveryMan::UpdateFSM(int worldTime, bool order)
{
	bool Day = false;
	
	if (worldTime - 1200 <= 0)
	{
		Day = true;
	}

	if (Day)
	{
		if (worldTime <= 600)
		{
			currentState = S_Sleeping;
		}

		else if (worldTime <= 1300 && worldTime >= 1200)
		{
			currentState = S_Eating;
		}

		/*
		Change to something like 
		else if( order != NULL)
		{
			currentState = S_Delivering;
			targetPath = order.path;
		}
		*/

		else if (order)
		{
			currentState = S_Delivering;
		}
	}

	else
	{
		if (worldTime >= 2300)
		{
			currentState = S_Sleeping;
		}

		else if ((worldTime <= 1300 && worldTime >= 1200) || (worldTime >= 1800 && worldTime <= 1900))
		{
			currentState = S_Eating;
		}

		/*
		Change to something like
		else if( order != NULL)
		{
		currentState = S_Delivering;
		targetPath = order.path;
		}
		*/

		else if (order)
		{
			currentState = S_Delivering;
		}
	}

	taskFinish = false;
}

void DeliveryMan::Draw(SceneManager* sceneManager)
{
	sceneManager->Render2DMesh(this->getMesh(), true, Vector2(1, 1), Vector2(this->getPosition().x, this->getPosition().y), 0);
}
