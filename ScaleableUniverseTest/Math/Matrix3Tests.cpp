#include "stdafx.h"

#include "Matrix3Tests.h"
#include "Source/Math/Fmath.h"
#include "Source/Math/Matrix3.h"


namespace ScaleableUniverseTest
{
	// Helper Functions
	ScaleableUniverse::Matrix3 Matrix3Tests::getRandomMatrix()
	{
		srand((unsigned int)time(NULL));
		ScaleableUniverse::Matrix3 result;
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				result.Set(i, j) = (rand() % 100 + 1) / 10.f;
			}
		}

		return result;
	}

	// TESTS
	bool Matrix3Tests::identityTest()
	{
		ScaleableUniverse::Matrix3 identity = ScaleableUniverse::Matrix3::identity();
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				float cVal = identity.Get(i, j);
				if ((i == j))
				{
					if (!ScaleableUniverse::fEquals(cVal, 1))
						return false;
				}
				else
				{
					if (!ScaleableUniverse::fEquals(cVal, 0))
					{
						return false;
					}
				}
			}
		}

		return true;
	}

	bool Matrix3Tests::matrixInverseTest(int numTimes)
	{
		for (int i = 0; i < numTimes; i++)
		{
			ScaleableUniverse::Matrix3 currentMatrix;
			// Generate a Random 3x3 Matrix
			while (ScaleableUniverse::fEquals(currentMatrix.determinant(), 0))
			{
				currentMatrix = getRandomMatrix();
			}

			bool testResult = ScaleableUniverse::Matrix3::identity().matrixFEquals((currentMatrix * currentMatrix.inverse()), 1E-5);
			if (!testResult)
			{
				return false;
			}
		}

		return true;
	}

};