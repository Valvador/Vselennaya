#include "Matrix3Tests.h"
#include "Source/Math/Fmath.h"
#include "Source/Math/Matrix3.h"



// Helper Functions
SAU::Matrix3 SAU::Matrix3Tests::getRandomMatrix()
{
	srand((unsigned int)time(NULL));
	SAU::Matrix3 result;
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
bool SAU::Matrix3Tests::identityTest()
{
	SAU::Matrix3 identity = SAU::Matrix3::identity();
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			float cVal = identity.Get(i, j);
			if ((i == j))
			{
				if (!SAU::fEquals(cVal, 1))
					return false;
			}
			else
			{
				if (!SAU::fEquals(cVal, 0))
				{
					return false;
				}
			}
		}
	}

	return true;
}

bool SAU::Matrix3Tests::matrixInverseTest(int numTimes)
{
	for (int i = 0; i < numTimes; i++)
	{
		SAU::Matrix3 currentMatrix;
		// Generate a Random 3x3 Matrix
		while (SAU::fEquals(currentMatrix.determinant(), 0))
		{
			currentMatrix = getRandomMatrix();
		}

		bool testResult = SAU::Matrix3::identity().matrixFEquals((currentMatrix * currentMatrix.inverse()), 1E-5);
		if (!testResult)
		{
			return false;
		}
	}

	return true;
}

