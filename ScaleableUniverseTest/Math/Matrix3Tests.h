#pragma once

#include "stdafx.h"
#include <stdlib.h>
#include <time.h>

namespace ScaleableUniverse
{
	class Matrix3;
}
namespace ScaleableUniverseTest
{
	class Matrix3Tests
	{
	private:
		// Helper Functions
		static ScaleableUniverse::Matrix3 getRandomMatrix();

	public:
		// TESTS
		static bool identityTest();
		static bool matrixInverseTest(int numTimes);
	};
}
