#include "stdafx.h"

#include "ReplicationInterface.h"
namespace SAU
{
	ReplicationInterface::ReplicationInterface(float timeout) :
		timeoutAccumulator(0),
		timeout(timeout)
	{
	}

	ReplicationInterface::~ReplicationInterface()
	{
	}

	bool ReplicationInterface::isTimedOutUpdate(float deltaTime)
	{
		timeoutAccumulator += deltaTime;
		if (timeoutAccumulator >= timeout)
		{
			timeoutAccumulator = 0.0f;
			return true;
		}

		return false;
	}

	void ReplicationInterface::processPacket(const unsigned char data[])
	{
		timeoutAccumulator = 0.0f;
	}

	void ReplicationInterface::update()
	{

	}
}
