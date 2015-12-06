#include "StoreMan.h"


StoreMan::StoreMan()
	: m_bTaskFinish(true)
	, m_bWaitingOrder(false)
	, currentState(S_IDLE)
	, m_bPathAssigned(false)
	, m_fMoveSpeed(150.f)
	, m_iReadLine(0)
	, WPData("")
	, m_cSplit_Char(',')
{
}


StoreMan::~StoreMan()
{
}

void StoreMan::Init()
{
	spriteAnim = new SpriteAnimation();

	ReadWayPoints_MoveOrder("Config\\Waypoints\\StoreMan\\MoveOrder.txt");
	ReadWayPoints_NewOrder("Config\\Waypoints\\StoreMan\\MakeOrder.txt");
	ReadWayPoints_StartPosition("Config\\Waypoints\\StoreMan\\StartPoint.txt");

	m_v2CurrentPos = Idle;
}

StoreMan::STATES StoreMan::getCurrentState(void)
{
	return this->currentState;
}

void StoreMan::ReadWayPoints_NewOrder(string fileName)
{
	//vector to contain elements split
	vector<string>WPTokens;

	//Reset line
	m_iReadLine = 0;

	//Load Level details
	std::ifstream inTxtFile;
	inTxtFile.open(fileName);
	if (inTxtFile.good())
	{
		while (getline(inTxtFile, WPData))
		{
			std::istringstream split(WPData);

			//Dont read lines with #
			if (WPData[0] == '#')
			{
				continue;
			}

			for (string each; std::getline(split, each, m_cSplit_Char);)
			{
				WPTokens.push_back(each);
			}

			//Load the points in
			AddWayPoints_NewOrder(Vector2(
				stof(WPTokens.at(X + m_iReadLine * POS_INDEX))
				, stof(WPTokens.at(Y + m_iReadLine * POS_INDEX))
				));

			++m_iReadLine;
		}
		WPTokens.clear();
		inTxtFile.close();
	}
	else
		std::cout << "Load Waypoint file failed" << std::endl;
}

void StoreMan::ReadWayPoints_MoveOrder(string fileName)
{
	//vector to contain elements split
	vector<string>WPTokens;

	//Reset line
	m_iReadLine = 0;

	//Load Level details
	std::ifstream inTxtFile;
	inTxtFile.open(fileName);
	if (inTxtFile.good())
	{
		while (getline(inTxtFile, WPData))
		{
			std::istringstream split(WPData);

			//Dont read lines with #
			if (WPData[0] == '#')
			{
				continue;
			}

			for (string each; std::getline(split, each, m_cSplit_Char);)
			{
				WPTokens.push_back(each);
			}

			//Load the points in
			AddWayPoints_MoveOrder(Vector2(
				stof(WPTokens.at(X + m_iReadLine * POS_INDEX))
				, stof(WPTokens.at(Y + m_iReadLine * POS_INDEX))
				));

			++m_iReadLine;
		}
		WPTokens.clear();
		inTxtFile.close();
	}
	else
		std::cout << "Load Waypoint file failed" << std::endl;
}

void StoreMan::ReadWayPoints_StartPosition(string fileName)
{

	{
		//vector to contain elements split
		vector<string>WPTokens;

		//Reset line
		m_iReadLine = 0;

		//Load Level details
		std::ifstream inTxtFile;
		inTxtFile.open(fileName);
		if (inTxtFile.good())
		{
			while (getline(inTxtFile, WPData))
			{
				std::istringstream split(WPData);

				//Dont read lines with #
				if (WPData[0] == '#')
				{
					continue;
				}

				for (string each; std::getline(split, each, m_cSplit_Char);)
				{
					WPTokens.push_back(each);
				}

				//Load the points in
				Idle = (Vector2(
					stof(WPTokens.at(X + m_iReadLine * POS_INDEX))
					, stof(WPTokens.at(Y + m_iReadLine * POS_INDEX))
					));

				++m_iReadLine;
			}
			WPTokens.clear();
			inTxtFile.close();
		}
		else
			std::cout << "Load Waypoint file failed" << std::endl;
	}
}

void StoreMan::AddWayPoints_NewOrder(Vector2 newWayPoint)
{
	NewOrder.push_back(newWayPoint);
}

void StoreMan::AddWayPoints_MoveOrder(Vector2 newWayPoint)
{
	MoveOrder.push_back(newWayPoint);
}

void StoreMan::Update(double dt, float* ingredients, bool* orderarrived, bool* waitOrder)
{
	switch (currentState)
	{
	case S_IDLE: 
		UpdateIdle(dt);
		break;
	case S_NEWORDER: // Go make new order
		UpdateMakeOrder(dt, waitOrder);
		break;
	case S_MOVEORDER: // Go move order
		UpdateMoveOrder(dt, ingredients);
		break;
	}

	if (m_bTaskFinish)
	{
		UpdateFSM(*ingredients, orderarrived, waitOrder);
	}

	spriteAnim->Update(dt);
	if (m_v2Direction.x == -1)
		this->spriteAnim->currentAni = WALK_LEFT;
	else if (m_v2Direction.x == 1)
		this->spriteAnim->currentAni = WALK_RIGHT;
	else if (m_v2Direction.y == 1)
		this->spriteAnim->currentAni = WALK_UP;
	else if (m_v2Direction.y == -1)
		this->spriteAnim->currentAni = WALK_DOWN;
	else
		this->spriteAnim->currentAni = WALK_DOWN;
}

void StoreMan::UpdateIdle(double dt)
{
	if (UpdatePath(Idle, false, dt))
	{
		m_bTaskFinish = true;
		m_v2Direction.SetZero();
	}
}

void StoreMan::UpdateMakeOrder(double dt, bool* waitOrder)
{
	if (UpdatePath(NewOrder, false, dt))
	{
		m_bTaskFinish = true;
		m_bWaitingOrder = true;
		*waitOrder = m_bWaitingOrder;
		m_v2Direction.SetZero();
	}
}

void StoreMan::UpdateMoveOrder(double dt, float* ingredients)
{
	if (UpdatePath(MoveOrder, false, dt))
	{
		m_bTaskFinish = true;
		*ingredients = 100.0f;
		m_v2Direction.SetZero();
	}
}

bool StoreMan::UpdatePath(vector<Vector2> PathToUpdate, bool Reverse, double dt)
{
	//Assign the start point of path
	if (!m_bPathAssigned)
	{
		if (Reverse)
		{
			m_iNextPoint = PathToUpdate.size() - 1;
		}
		else
		{
			m_iNextPoint = 0;

		}
		m_bPathAssigned = true;
	}

	m_v2NextPos = PathToUpdate.at(m_iNextPoint);

	//Continue to update if have not reached next waypoint
	if (m_v2CurrentPos != PathToUpdate.at(m_iNextPoint))
	{
		m_v2Direction = (m_v2NextPos - m_v2CurrentPos).Normalized();

		//Getting distance
		m_fDistSquared = ((m_v2NextPos.x - m_v2CurrentPos.x) * (m_v2NextPos.x - m_v2CurrentPos.x) +
			(m_v2NextPos.y - m_v2CurrentPos.y) * (m_v2NextPos.y - m_v2CurrentPos.y));

		//Moving to position
		m_v2CurrentPos += m_v2Direction * ((float)dt * m_fMoveSpeed);

		//Comparing if destination has been reached
		if (m_fDistSquared < ((dt * m_fMoveSpeed) * (dt * m_fMoveSpeed)))
		{
			m_v2CurrentPos = m_v2NextPos;
		}
	}
	//Go to next waypoint
	else
	{
		if (!Reverse)
			++m_iNextPoint;
		else
			--m_iNextPoint;

		//Finished moving through waypoints
		if (m_iNextPoint == PathToUpdate.size())
		{
			m_bPathAssigned = false;
			return true;
		}
		if (m_iNextPoint < 0)
		{
			m_bPathAssigned = false;
			return true;
		}
	}
	return false;
}

bool StoreMan::UpdatePath(Vector2 PathToUpdate, bool Reverse, double dt)
{
	m_v2NextPos = PathToUpdate;

	//Continue to update if have not reached next waypoint
	if (m_v2CurrentPos != m_v2NextPos)
	{
		m_v2Direction = (m_v2NextPos - m_v2CurrentPos).Normalized();

		//Getting distance
		m_fDistSquared = ((m_v2NextPos.x - m_v2CurrentPos.x) * (m_v2NextPos.x - m_v2CurrentPos.x) +
			(m_v2NextPos.y - m_v2CurrentPos.y) * (m_v2NextPos.y - m_v2CurrentPos.y));

		//Moving to position
		m_v2CurrentPos += m_v2Direction * ((float)dt * m_fMoveSpeed);

		//Comparing if destination has been reached
		if (m_fDistSquared < ((dt * m_fMoveSpeed) * (dt * m_fMoveSpeed)))
		{
			m_v2CurrentPos = m_v2NextPos;
			return true;
		}
	}
	return false;
}

void StoreMan::UpdateFSM(float ingredients, bool* orderarrived, bool* waitOrder)
{
	if (m_bWaitingOrder && *orderarrived)
	{
		currentState = S_MOVEORDER;
		m_bTaskFinish = false;
		*orderarrived = false;
		m_bWaitingOrder = false;
		*waitOrder = m_bWaitingOrder;
	}

	else if (ingredients <= 20 && !m_bWaitingOrder)
	{
		currentState = S_NEWORDER;
		m_bTaskFinish = false;
	}
	
	else
	{
		currentState = S_IDLE;
	}
}

void StoreMan::Draw(SceneManager* sceneManager)
{
	sceneManager->Render2DMesh(spriteAnim, true, Vector2(50, 50), m_v2CurrentPos);
}


void StoreMan::SetSpriteAnim(SpriteAnimation* NewSpriteAnim)
{
	*(this->spriteAnim) = *NewSpriteAnim;
	this->spriteAnim->currentAni = WALK_DOWN;
}

Vector2 StoreMan::GetPosition()
{
	return m_v2CurrentPos;
}