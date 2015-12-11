#include "Barista.h"
//Idle until customer is buying

//Check if enough ingredients

//If enough, proceed to brew

//Continue to brew is there are still customers and enough ingredients

//If not enough, proceed to refill

//If there are still customers continue to brew

Barista::Barista()
: currentState(S_IDLE)

, m_bNeedToRefill(false)
, m_bNeedToBrew(false)
, m_bPathAssigned(false)

, m_iNextPoint(0)
, m_iNumOrders(0)
, m_iBrewBar(0)
, m_iDrinksPrepared(0)

, m_fBrewTimer(1.f)
, m_fBrewProgress(0.f)
, m_fRefillTimer(1.f)
, m_fRefillProgress(0.f)
, m_fMoveSpeed(500.f)
, m_fDistSquared(0.f)

//Text file reading
, m_iReadLine(0)
, WPData("")
, m_cSplit_Char(',')
{
}

Barista::~Barista()
{
}

void Barista::Init(void)
{
	spriteAnim = new SpriteAnimation();

	ReadWayPoints_Refill("Config\\Waypoints\\Barista\\Refill_1.txt");
	ReadWayPoints_Brew("Config\\Waypoints\\Barista\\Brew_1.txt");
	
	//Initialise at sleeping position first
	m_v2CurrentPos = Brew.at(0);
}

void Barista::ReadWayPoints_Refill(string fileName)
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
			AddWayPoints_Refill(Vector2(
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

void Barista::ReadWayPoints_Brew(string fileName)
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

			//Load the points in based on the type
			AddWayPoints_Brew(Vector2(
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

void Barista::Update(double dt, float& ingredients, float& trash, float& reserve)
{
	//Need to start brewing if there is a order
	switch (currentState)
	{
	case S_IDLE:
		m_v2Direction.Set(0, -1);
		UpdateIdle(dt);
		break;
	case S_REFILL:
		m_v2Direction.Set(0, -1);
		UpdateRefill(dt, ingredients, reserve);
		break;
	case S_BREW:
		m_v2Direction.Set(0, -1);
		UpdateBrew(dt, ingredients, trash);
		break;
	default:
		break;
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

void Barista::UpdateIdle(double dt)
{
	//Continue to brew as long as there are orders
	if (m_iNumOrders > 0)
	{
		//Move to counter
		if (UpdatePath(Brew, false, dt))
		{
			currentState = S_BREW;
		}
	}
	//Go back to idling spot
	if (m_iNumOrders == 0)
	{
		if (m_v2CurrentPos != Brew.at(0))
			UpdatePath(Brew, true, dt);
	}
}
void Barista::UpdateRefill(double dt, float& ingredients, float& reserve)
{
	m_fRefillProgress += dt;

	//Refill in units of 20 every second
	if (m_fRefillProgress >= m_fRefillTimer)
	{
		if (reserve > 0)
		{
			ingredients += 20;
			reserve -= 20;
		}
		m_fRefillProgress = 0;
	}

	//Proceed to other states once fully replenished ingredients
	if (ingredients == 100)
	{
		if (m_iNumOrders > 0)
		{
			//Move back to counter
			if (UpdatePath(Refill, true, dt))
			{
				currentState = S_BREW;
			}
		}
		else
		{
			//Move back to idle
			if (UpdatePath(Brew, true, dt))
			{
				currentState = S_IDLE;
			}
		}
	}
}
void Barista::UpdateBrew(double dt, float& ingredients, float& trash)
{
	m_fBrewProgress += dt;
	++m_iBrewBar;

	//Brew once every second
	if (m_fBrewProgress >= m_fBrewTimer)
	{
		--m_iNumOrders;
		ingredients -= 5;
		trash += 5;
		m_fBrewProgress = 0;
		m_iBrewBar = 0;
		++m_iDrinksPrepared;
	}

	//Return to idle if there are no more orders
	if (m_iNumOrders == 0)
	{
		currentState = S_IDLE;
		//Move back to idle
		UpdatePath(Brew, true, dt);
	}
	if (ingredients == 0)
	{
		if (UpdatePath(Refill, false, dt))
		{
			currentState = S_REFILL;
		}
	}
}

int Barista::GetNumDrinks(void)
{
	return m_iDrinksPrepared;
}

bool Barista::GetDrinkPrepared(void)
{
	if (m_iDrinksPrepared > 0)
		return true;
	else 
		return false;
}

void Barista::SubtractDrinkPrepared(void)
{
	--m_iDrinksPrepared;
}

Vector2 Barista::GetPos(void)
{
	return m_v2CurrentPos;
}

Vector2 Barista::GetDir(void)
{
	return m_v2Direction;
}

void Barista::addNumOrders(const int numOrders)
{
	this->m_iNumOrders += numOrders;
}

int Barista::getNumOrders(void)
{
	return m_iNumOrders;
}

void Barista::AddWayPoints_Refill(Vector2 newWayPoint)
{
	Refill.push_back(newWayPoint);
}
void Barista::AddWayPoints_Brew(Vector2 newWayPoint)
{
	Brew.push_back(newWayPoint);
}

bool Barista::UpdatePath(vector<Vector2> PathToUpdate, bool Reverse, double dt)
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

void Barista::Draw(SceneManager* sceneManager)
{
	sceneManager->Render2DMesh(this->getMesh(), true, Vector2(1, 1), Vector2(this->getPosition().x, this->getPosition().y), 0);
}

Barista::STATES Barista::getCurrentState(void)
{
	return currentState;
}

void Barista::SetSpriteAnim(SpriteAnimation* newSprite)
{
	*(this->spriteAnim) = *newSprite;
	this->spriteAnim->currentAni = WALK_DOWN;
}

SpriteAnimation* Barista::GetSpriteAnim(void)
{
	return spriteAnim;
}