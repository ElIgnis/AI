#include "Customer.h"

Customer::Customer(Vector2 startPos) :
		currentState(S_WALKING),
		m_fDistanceSq(0.f),
		m_bActive(false),
		m_bOutdoor(true)
{
	m_v2CurrentPos = startPos;
	m_v2ShopPos = Vector2(1080, 280);
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
		if (m_v2CurrentPos == m_v2ShopPos)
		{
			currentState = S_DECIDE;
		}
		else
		{
			currentState = S_WALKING;
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
		if (CalculateProbability(worldTime, weather) > 50)
		{
			//Enter shop
			m_bOutdoor = false;
			currentState = S_WALKING;
		}
		else
		{
			currentState = S_WALKING;
		}
		break;
	case S_QUEUE:
		break;
	case S_BUY:
		break;
	case S_WAIT:
		break;
	case S_PICKUP:
		break;
	case S_STAY:
		break;
	}
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