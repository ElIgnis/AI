#include "GenericAI.h"

GenericAI::GenericAI(Roles role)
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
		if (mb->GetMsg(RC_TO_DELIVERYMAN))
		{
			currentRole = DELIVERY_MAN;
		}
		break;
	case DELIVERY_MAN:
		if (mb->GetMsg(RC_TO_BARISTA))
		{
			deliveryMan->setRC_Barista(true);
		}
		if (deliveryMan->getRC_Completed())
		{
			currentRole = BARISTA;
			barista->SetPos(deliveryMan->GetPos());
			barista->setCurrentState(Barista::S_REFILL);
			//barista->addNumOrders(1);
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