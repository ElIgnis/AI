#pragma once

#include "Barista.h"
#include "DeliveryMan.h"
#include "RubbishMan.h"
#include "StoreMan.h"

class GenericAI
{
public:
	enum Roles{
		BARISTA = 0,
		DELIVERY_MAN,
		RUBBISH_MAN,
		STORE_MAN,
		NUM_ROLES
	};

	GenericAI(Roles role);
	~GenericAI();

	DeliveryMan* deliveryMan;
	Barista* barista;
	RubbishMan* rubbishMan;
	StoreMan* storeMan;

	Roles GetOriginalRole(void)
	{
		return originalRole;
	}
	Roles GetCurrentRole(void)
	{
		return currentRole;
	}
	void UpdateRoleChange(MessageBoard* mb);

private:
	Roles originalRole;
	Roles currentRole;

	bool RCtoBarista;
	bool RCtoDeliveryMan;

};

