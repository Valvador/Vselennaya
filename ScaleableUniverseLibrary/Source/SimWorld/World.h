#pragma once
#include <vector>

namespace ScaleableUniverse
{
	class Actor;
	
	class World
	{
	private:
		std::vector<Actor*> actors;
	};
};