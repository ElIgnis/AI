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
		m_bInQueue(false)
{
	m_v2CurrentPos = startPos;
	m_v2ShopPos = Vector2(1080, 280);
	m_v2BuyPos = Vector2(1080, 600);
	m_v2WaitPos = Vector2(800, 500);
	m_v2PickupPos = Vector2(1000, 600);
	currentSprite = new SpriteAnimation();
}
Customer::~Customer()
{

}

void Customer::Update(double dt, int worldTime, int weather)
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
			else if (m_v2CurrentPos == m_v2WaitPos)
			{
				currentState = S_WAIT;
			}
			else if (m_v2CurrentPos == m_v2PickupPos)
			{
				currentState = S_PICKUP;
			}
			else
			{
				currentState = S_QUEUE;
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
		}
		break;
	case S_DECIDE:
		m_bDeciding = true;
		m_fDelay += dt;
		if (m_fDelay > 0.3f)
		{
			if (CalculateProbability(worldTime, weather) > 50)
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
		}
		break;
	case S_BUY:	
		m_fDelay += dt;
		if (m_fDelay > 0.5f)
		{
			//Walk to wait position
			currentState = S_WALKING;
			m_bQueue = false;
			m_fDelay = 0.f;
		}		
		break;
	case S_WAIT:
		m_fDelay += dt;
		if (m_fDelay > 1.0f)
		{
			//Walk to wait position
			currentState = S_WALKING;
			m_fDelay = 0.f;
		}
		break;
	case S_PICKUP:
		m_fDelay += dt;
		if (m_fDelay > 0.1f)
		{
			m_bPickedUp = true;
			//Walk to wait position
			currentState = S_WALKING;
			m_fDelay = 0.f;
		}
		break;
	case S_STAY:
		break;
	}

	currentSprite->Update(dt);
}

void Customer::UpdateFSM()
{
	
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

int Customer::CalculateProbability(int time, int weather)
{
	return Math::RandIntMinMax(1, 100);
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