#include "GenericAI.h"

GenericAI::GenericAI(Roles role)
: RCtoBarista(false)
, RCtoDeliveryMan(false)
{
	//Creates an instance of all AIs
	deliveryMan = new DeliveryMan();
	barista = new Barista();
	rubbishMan = new RubbishMan();
	storeMan = new StoreMan();

	//Assigns original role
	originalRole = role;
	currentRole = role;

	//Initialise all AIs
	deliveryMan->Init();
	barista->Init();
	rubbishMan->Init();
	storeMan->Init();
}


GenericAI::~GenericAI()
{
	if (deliveryMan)
	{
		delete deliveryMan;
		deliveryMan = NULL;
	}
	if (barista)
	{
		delete barista;
		barista = NULL;
	}
	if (rubbishMan)
	{
		delete rubbishMan;
		rubbishMan = NULL;
	}
	if (storeMan)
	{
		delete storeMan;
		storeMan = NULL;
	}
}

void GenericAI::UpdateRoleChange(MessageBoard* mb)
{
	switch (currentRole)
	{
	case BARISTA:
		if (!RCtoDeliveryMan && mb->GetMsg(RC_TO_DELIVERYMAN))
		{
			barista->setRC_DeliveryMan(true);
			RCtoDeliveryMan = true;
		}
		if (barista->getRC_Completed())
		{
			currentRole = DELIVERY_MAN;
			deliveryMan->SetPos(barista->GetPos());
			deliveryMan->setCurrentState(DeliveryMan::S_COLLECTING);
			deliveryMan->setRC_Completed(false);
			deliveryMan->setNeedToSleep(false);
			RCtoDeliveryMan = false;
		}
		break;
	case DELIVERY_MAN:
		if (!RCtoBarista && !deliveryMan->getNeedToEat() && !deliveryMan->getNeedToSleep() && !deliveryMan->getOrderToCollect())
		{
			if (mb->GetMsg(RC_TO_BARISTA))
			{
				deliveryMan->setRC_Barista(true);
				RCtoBarista = true;
			}
		}
		if (deliveryMan->getRC_Completed())
		{
			currentRole = BARISTA;
			barista->SetPos(deliveryMan->GetPos());
			barista->setCurrentState(Barista::S_REFILL);
			barista->setRC_Completed(false);
			RCtoBarista = false;
		}
		break;
	case RUBBISH_MAN:
		break;
	case STORE_MAN:
		break;
	default:
		break;
	}
}