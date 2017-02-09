#include "stdafx.h"
#include "Guid.h"
#include <assert.h>

namespace ScaleableUniverse
{
	void GuidGenerator::initGuidGen()
	{
		lastUsed = 0;
	}

	int GuidGenerator::generateGuid()
	{
		return lastUsed++;
	}
};