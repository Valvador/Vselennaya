#include "stdafx.h"
#include "Matrix3Tests.h"

#include "Source/Math/Fmath.h"
#include "Source/Math/Matrix3.h"

using namespace ScaleableUniverseTest;
using namespace ScaleableUniverse;

// Helper Functions
Matrix3 Matrix3Tests::getRandomMatrix()
{
	srand((unsigned int)time(NULL));
	Matrix3 result;
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
	Matrix3 identity = Matrix3::identity();
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			float cVal = identity.Get(i, j);
			if ((i == j))
			{
				if (!fEquals(cVal, 1))
					return false;
			}
			else
			{
				if (!fEquals(cVal, 0))
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
		Matrix3 currentMatrix;
		// Generate a Random 3x3 Matrix
		while (fEquals(currentMatrix.determinant(), 0))
		{
			currentMatrix = getRandomMatrix();
		}

		bool testResult = Matrix3::identity().matrixFEquals((currentMatrix * currentMatrix.inverse()), 1E-5);
		if (!testResult)
		{
			return false;
		}
	}

	return true;
}