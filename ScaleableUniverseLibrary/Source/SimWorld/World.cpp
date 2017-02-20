#include "stdafx.h"
#include "World.h"


namespace SAU
{
	void World::stepWorld()
	{
		sim->stepSimulator(actors);
	}
};