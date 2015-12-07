#include "RubbishMan.h"


RubbishMan::RubbishMan()
	: m_bTaskFinish(true)
	, currentState(S_IDLE)
	, m_bNeedToEat(false)
	, m_bPathAssigned(false)
	, m_bSeated(false)
	, m_fMoveSpeed(150.f)
	, m_iActionDuration(0)
	, m_iReadLine(0)
	, WPData("")
	, m_cSplit_Char(',')
{
}


RubbishMan::~RubbishMan()
{
}

void RubbishMan::Init()
{
	spriteAnim = new SpriteAnimation();

	ReadWayPoints_Eat("Config\\Waypoints\\RubbishMan\\Eat.txt");
	ReadWayPoints_TakeTrash("Config\\Waypoints\\RubbishMan\\TakeTrash.txt");
	ReadWayPoints_StartPosition("Config\\Waypoints\\RubbishMan\\StartPoint.txt");

	m_v2CurrentPos = Idle;
}

RubbishMan::STATES RubbishMan::getCurrentState(void)
{
	return this->currentState;
}

void RubbishMan::Update(double dt, int worldTime, float* trash)
{
	// Need to eat at 1200 hours
	if (worldTime == 1200 || worldTime == 1800)
	{
		m_bNeedToEat = true;
	}

	switch (currentState)
	{
	case S_IDLE:
		UpdateIdle(dt);
		break;
	case S_EAT:
		UpdateEat(dt, worldTime);
		break;
	case S_TAKETRASH:
		UpdateTakeTrash(dt, trash);
		break;
	}

	if (m_bTaskFinish)
	{
		UpdateFSM(worldTime, *trash);
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

void RubbishMan::UpdateFSM(int worldTime, float trash)
{
	if (trash >= 80)
	{
		currentState = S_TAKETRASH;
		m_bTaskFinish = false;
	}
	else if (m_bNeedToEat)
	{
		currentState = S_EAT;
		m_bTaskFinish = false;
	}
	else
	{
		currentState = S_IDLE;
	}
}

void RubbishMan::UpdateIdle(double dt)
{
	if (UpdatePath(Idle, false, dt))
	{
		m_bTaskFinish = true;
		m_v2Direction.SetZero();
	}
}

void RubbishMan::UpdateTakeTrash(double dt, float* trash)
{
	if (UpdatePath(TakeTrash, false, dt))
	{
		m_bTaskFinish = true;
		*trash = 0.f;
		m_v2Direction.SetZero();
	}
}

void RubbishMan::UpdateEat(double dt, int worldTime)
{
	if (!m_bSeated)
	{
		if (UpdatePath(Eat, false, dt))
		{
			m_bSeated = true;
			if (m_iActionDuration == 0)
				m_iActionDuration = worldTime;
		}
	}

	else
	{
		int tempConversion = worldTime - m_iActionDuration;

		if (tempConversion < 0)
		{
			tempConversion += 2400;
		}

		//Finish eating after 1 hour
		if (tempConversion >= 100)
		{
			if (UpdatePath(Eat, true, dt))
			{
				m_bNeedToEat = false;
				m_bTaskFinish = true;
				m_bSeated = false;
				m_iActionDuration = 0;
				m_v2Direction.SetZero();
			}
		}
	}
}

void RubbishMan::Draw(SceneManager* sceneManager)
{
	sceneManager->Render2DMesh(spriteAnim, true, Vector2(50, 50), m_v2CurrentPos);
}

void RubbishMan::ReadWayPoints_Eat(string fileName)
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
			AddWayPoints_Eat(Vector2(
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

void RubbishMan::ReadWayPoints_TakeTrash(string fileName)
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
			AddWayPoints_TakeTrash(Vector2(
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

void RubbishMan::ReadWayPoints_StartPosition(string fileName)
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

void RubbishMan::AddWayPoints_Eat(Vector2 newWayPoint)
{
	Eat.push_back(newWayPoint);
}

void RubbishMan::AddWayPoints_TakeTrash(Vector2 newWayPoint)
{
	TakeTrash.push_back(newWayPoint);
}

bool RubbishMan::UpdatePath(vector<Vector2> PathToUpdate, bool Reverse, double dt)
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

bool RubbishMan::UpdatePath(Vector2 PathToUpdate, bool Reverse, double dt)
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

void RubbishMan::SetSpriteAnim(SpriteAnimation* NewSpriteAnim)
{
	*(this->spriteAnim) = *NewSpriteAnim;
	this->spriteAnim->currentAni = WALK_DOWN;
}

Vector2 RubbishMan::GetPosition()
{
	return m_v2CurrentPos;
}
