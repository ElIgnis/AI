#include "Customer.h"

Customer::Customer(Vector2 startPos) :
		currentState(S_WALKING),
		m_fDistanceSq(0.f),
		m_bActive(false),
		m_bOutdoor(true),
		m_fDelay(0.f),
		m_bDeciding(false),
		m_bPickedUp(false),
		m_bQueue(false),
		m_bInQueue(false),
		m_bDrinkAvailable(false),
		m_bOrderPlaced(false),
		m_bWait(false),
		m_bInWait(false),
		m_bRandomUrgent(false),
		m_bAbleToCut(false),
		m_QueueID(-1),
		m_IDtoGO(-1)
{
	m_v2CurrentPos = startPos;
	m_v2ShopPos = Vector2(1080, 280);
	m_v2BuyPos = Vector2(1150, 600);
	m_v2WaitPos = Vector2(900, 550);
	m_v2PickupPos = Vector2(1000, 600);
	currentSprite = new SpriteAnimation();
}
Customer::~Customer()
{

}

void Customer::Update(double dt, int worldTime, int weather, MessageBoard *mb)
{
	Vector2 direction = (m_v2NextPos - m_v2CurrentPos).Normalized();
	switch (currentState)
	{
	case S_IDLE:
		//No drink bought
		if (!m_bPickedUp && m_bOutdoor)
		{
			if (m_v2CurrentPos == m_v2ShopPos)
			{
				currentState = S_DECIDE;
			}
			else
			{
				currentState = S_WALKING;
			}
		}
		//In shop but no drink bought
		else if (!m_bPickedUp && !m_bOutdoor)
		{
			if (m_v2CurrentPos == m_v2BuyPos)
			{
				currentState = S_BUY;
			}
			else if (m_v2CurrentPos == m_v2PickupPos)
			{
				currentState = S_PICKUP;
			}
			else if (m_bQueue)
			{
				currentState = S_QUEUE;
			}
			else
			{
				currentState = S_WAIT;
			}
		}
		else
		{
			if (m_v2CurrentPos == m_v2ShopPos)
			{
				m_bOutdoor = true;
				currentState = S_WALKING;
			}
			else
			{
				currentState = S_WALKING;
			}
		}
		break;
	case S_WALKING:
		if (m_v2CurrentPos != m_v2NextPos)
		{
			//Getting distance
			m_fDistanceSq = ((m_v2NextPos.x - m_v2CurrentPos.x) *
				(m_v2NextPos.x - m_v2CurrentPos.x) +
				(m_v2NextPos.y - m_v2CurrentPos.y) *
				(m_v2NextPos.y - m_v2CurrentPos.y));

			//Moving to position
			m_v2CurrentPos += direction * (dt * m_fSpeed);
			//Comparing if destination has been reached
			if (m_fDistanceSq < ((dt * m_fSpeed) * (dt * m_fSpeed)))
			{
				m_v2CurrentPos = m_v2NextPos;
				currentState = S_IDLE;
			}
			currentSprite->Update(dt);
		}
		break;
	case S_DECIDE:
		m_bDeciding = true;
		m_fDelay += dt;
		if (m_fDelay > 0.3f)
		{
			if (CalculateProbability(worldTime, weather))
			{
				//Enter shop
				m_bOutdoor = false;
				m_bQueue = true; //Start queuing
				currentState = S_WALKING;
				m_bDeciding = false;
				m_fDelay = 0.f;
			}
			else
			{
				//Stay outside
				m_bDeciding = false;
				currentState = S_WALKING;
				m_fDelay = 0.f;
			}
		}
		break;
	case S_QUEUE:
		if (m_v2CurrentPos != m_v2NextPos)
		{
			currentState = S_WALKING;

			//Only if customer is able to Cut the queue and has not randomed his urgent probability
			if (m_bAbleToCut && !m_bRandomUrgent)
			{
				//Calculate Customer's Urgent Probability
				if (CalculateUrgentProbability())
				{
					mb->AddMessage(Urgent, std::to_string(m_QueueID), std::to_string(m_IDtoGO));
				}
			}
		}
		break;
	case S_BUY:	
		m_fDelay += dt;
		if (m_fDelay > 0.5f)
		{
			//Walk to wait position
			currentState = S_WALKING;
			m_bOrderPlaced = true;
			m_bQueue = false;
			m_bWait = true;
			m_fDelay = 0.f;
		}		
		break;
	case S_WAIT:
		if (m_v2CurrentPos == m_v2WaitPos)
		{
			m_bWait = false;
			if (m_bDrinkAvailable == true)
			{
				//Walk to pick up position
				currentState = S_WALKING;
			}
		}
		else if (m_v2CurrentPos != m_v2NextPos)
		{
			currentState = S_WALKING;
		}
		break;
	case S_PICKUP:
		m_fDelay += (float)dt;
		if (m_bPickedUp == true)
		{
			m_fDelay = 0.f;
			currentState = S_WALKING;
		}
		break;
	}
}

Vector2 Customer::getCurrentPos()
{
	return m_v2CurrentPos;
}
void Customer::setStartPos(Vector2 startpos)
{
	this->m_v2CurrentPos = startpos;
}
void Customer::setNextPoint(Vector2 nextpoint)
{
	m_v2NextPos = nextpoint;
}

Customer::STATES Customer::getCurrentState(void)
{
	return currentState;
}

float Customer::getDelay()
{
	return m_fDelay;
}

void Customer::setPickedUp(bool pickup)
{
	this->m_bPickedUp = pickup;
}

bool Customer::getInWaitStatus()
{
	return this->m_bInWait;
}
void Customer::setInWaitStatus(bool wait)
{
	this->m_bInWait = wait;
}

bool Customer::CalculateUrgentProbability(void)
{
	int prob = Math::RandIntMinMax(1, 100);
	if (prob >= 50)
	{
		m_bRandomUrgent = true;
		return true;
	}
	else
	{
		return false;
	}
}
bool Customer::CalculateProbability(int time, int weather)
{
	int chance = Math::RandIntMinMax(1, 100);
	//Peak hour
	if (time >= 1600 && time <= 1900)
	{
		if (chance > 1)	{
			return true;
		}
		else{
			return false;
		}
	}
	//Normal timing
	else{
		if (chance > 1)	{
			return true;
		}
		else{
			return false;
		}
	}
}

void Customer::setActive(bool active)
{
	this->m_bActive = active;
}
bool Customer::getActive()
{
	return this->m_bActive;
}

bool Customer::getOutdoorStatus()
{
	return this->m_bOutdoor;
}
bool Customer::getQueueStatus()
{
	return m_bQueue;
}

bool Customer::getWaitStatus()
{
	return this->m_bWait;
}
bool Customer::getInQueueStatus()
{
	return this->m_bInQueue;
}
void Customer::setInQueueStatus(bool inqueue)
{
	this->m_bInQueue = inqueue;
}

void Customer::setSprite(SpriteAnimation* sprite)
{
	*(this->currentSprite) = *sprite;
	this->currentSprite->currentAni = WALK_DOWN;
}
SpriteAnimation* Customer::getSprite(void)
{
	return this->currentSprite;
}

Customer::STATES Customer::getState(void)
{
	return this->currentState;
}
void Customer::setState(STATES state)
{
	this->currentState = state;
}

void Customer::setDrinkAvailable(bool available)
{
	this->m_bDrinkAvailable = available;
}

void Customer::setOrderPlaced(bool placed)
{
	this->m_bOrderPlaced = placed;
}
bool Customer::getOrderPlaced(void)
{
	return this->m_bOrderPlaced;
}

void Customer::SetQueueID(int id)
{
	this->m_QueueID = id;
}
int Customer::GetQueueID(void)
{
	return this->m_QueueID;
}

Vector2 Customer::getPos()
{
	return this->m_v2CurrentPos;
}

void Customer::setCutQueueStatus(bool status, int IDtoGO)
{
	this->m_bAbleToCut = status;
	this->m_IDtoGO = IDtoGO;
}
bool Customer::getCutQueueStatus(void)
{
	return this->m_bAbleToCut;
}

Vector2 Customer::getNextPoint(void)
{
	return this->m_v2NextPos;
}

void Customer::Reset(void)
{
	currentState = S_WALKING;
	m_fDistanceSq = 0.f;
	m_bActive = false;
	m_bOutdoor = true;
	m_fDelay = 0.f;
	m_bDeciding = false;
	m_bPickedUp = false;
	m_bQueue = false;
	m_bInQueue = false;
	m_bDrinkAvailable = false;
	m_bOrderPlaced = false;
	m_bInWait = false;
	m_bRandomUrgent = false;
	m_bAbleToCut = false;
	m_QueueID = -1;
	m_IDtoGO = -1;
}