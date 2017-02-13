#include "stdafx.h"
#include "World.h"


namespace ScaleableUniverse
{
	void World::stepWorld()
	{
		sim->stepSimulator(actors);
	}
};