#include "stdafx.h"
#include "Actor.h"
#include "../Util/Guid.h"

namespace SAU
{
	// Constructors
	Actor::Actor() : 
		position(0, 0, 0),
		linearVelocity(0, 0, 0),
		angularVelocity(0, 0, 0),
		guid(GuidGenerator::generateGuid())
	{

	}

	// Data Access
	void Actor::updateCoords(const Vector3D& pos, const Matrix3& rot)
	{
		position = pos; 
		rotation = rot;
	}

	void Actor::updateVelocities(const Vector3D& linVel, const Vector3D& angularVel)
	{
		linearVelocity = linVel;
		angularVelocity = angularVel;
	}

	void Actor::updatePhysics(const Vector3D& pos, const Matrix3& rot, const Vector3D& linVel, const Vector3D& angularVel)
	{
		updateCoords(pos, rot);
		updateVelocities(linVel, angularVel);
	}
}