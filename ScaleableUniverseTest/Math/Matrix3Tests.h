#ifndef Matrix3Test_H
#define Matrix3Test_H

#include <stdlib.h>
#include <time.h>

namespace SAU
{
	class Matrix3;
	class Matrix3Tests
	{
	private:
		// Helper Functions
		static SAU::Matrix3 getRandomMatrix();

	public:
		// TESTS
		static bool identityTest();
		static bool matrixInverseTest(int numTimes);
	};
}

#endif
