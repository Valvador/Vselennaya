#pragma once
#include <vector>
#include "Simulator.h"

namespace ScaleableUniverse
{
	class Actor;
	
	class World
	{
	private:
		// Data
		float					worldScale;				//For when we implement large differences in World scales
		std::vector<Actor*>		actors;
		Simulator*				sim;
		

		// Step
		void stepWorld();

	public:
		// Constructors
		World(float scale, Simulator* simulator) :
			worldScale(scale),
			sim(simulator)
		{};

		~World()
		{
			delete sim;
		}
	};
};