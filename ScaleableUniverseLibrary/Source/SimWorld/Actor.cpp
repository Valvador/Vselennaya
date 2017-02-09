#include "stdafx.h"
#include "Actor.h"
#include "../Util/Guid.h"

namespace ScaleableUniverse
{
	Actor::Actor() : 
		guid(GuidGenerator::generateGuid())
	{

	}
}