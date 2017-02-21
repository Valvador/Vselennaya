#ifndef REPINT_H
#define REPINT_H

#include "Net.h"
#include <vector>

namespace SAU
{
	class ReplicationInterface
	{
	public:
		ReplicationInterface(float timeout);
		~ReplicationInterface();

		// Data Update Methods
		bool isTimedOutUpdate(float deltaTime);

		// Virtual Methods
		virtual void processPacket(const unsigned char data[]);
		virtual void update();
	private:
		float timeout;
		float timeoutAccumulator;
	};
}

#endif