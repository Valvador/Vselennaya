#pragma once
#include "../Math/Matrix3.h"
#include "../ThirdParty/Math3DLib/Vector3D.h"

namespace ScaleableUniverse
{
	class Actor
	{
	private:
		// Fields
		int guid;
		
		// Physics Requirements
		Vector3D position;
		Matrix3  rotation;
		Vector3D linearVelocity;
		Vector3D angularVelocity;

	public:
		// Constructors
		Actor();

		// Data Access
		void updateCoords(const Vector3D& position, const Matrix3& rotation);
		void updateVelocities(const Vector3D& linVel, const Vector3D& angularVel);
		void updatePhysics(const Vector3D& position, const Matrix3& rotation, const Vector3D& linVel, const Vector3D& angularVel);
	};
};