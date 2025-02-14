#include "GameObject3D.h"

GameObject3D::GameObject3D()
{
}

GameObject3D::~GameObject3D()
{
}

void GameObject3D::setHitbox(OBB3D hitbox)
{
	this->obbHitbox = hitbox;
}

OBB3D GameObject3D::getHitbox(void)
{
	return this->obbHitbox;
}

void GameObject3D::updateHitbox(void)
{
	obbHitbox.setMidPoint(this->position);
	Mtx44 vertex = this->getProperties().modelProperties;

	obbHitbox.create3Dhitbox
		(vertex * Vector3(obbHitbox.getHalfLength(), -obbHitbox.getHalfHeight(), obbHitbox.getHalfDepth()) + obbHitbox.getMidPoint(),
		vertex * Vector3(obbHitbox.getHalfLength(), obbHitbox.getHalfHeight(), obbHitbox.getHalfDepth()) + obbHitbox.getMidPoint(),
		vertex * Vector3(-obbHitbox.getHalfLength(), obbHitbox.getHalfHeight(), obbHitbox.getHalfDepth()) + obbHitbox.getMidPoint(),
		vertex * Vector3(-obbHitbox.getHalfLength(), -obbHitbox.getHalfHeight(), obbHitbox.getHalfDepth()) + obbHitbox.getMidPoint(),
		vertex * Vector3(obbHitbox.getHalfLength(), -obbHitbox.getHalfHeight(), -obbHitbox.getHalfDepth()) + obbHitbox.getMidPoint(),
		vertex * Vector3(obbHitbox.getHalfLength(), obbHitbox.getHalfHeight(), -obbHitbox.getHalfDepth()) + obbHitbox.getMidPoint(),
		vertex * Vector3(-obbHitbox.getHalfLength(), obbHitbox.getHalfHeight(), -obbHitbox.getHalfDepth()) + obbHitbox.getMidPoint(),
		vertex * Vector3(-obbHitbox.getHalfLength(), -obbHitbox.getHalfHeight(), -obbHitbox.getHalfDepth()) + obbHitbox.getMidPoint(), obbHitbox.getName()
		);
}