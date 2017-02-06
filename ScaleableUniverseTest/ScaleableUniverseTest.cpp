#pragma once

#include "stdafx.h"
#include <assert.h>
#include "Math/Matrix3Tests.h"

using namespace ScaleableUniverseTest;

int main()
{

	assert(Matrix3Tests::identityTest());

	assert(Matrix3Tests::matrixInverseTest(100));

	return 0;
}

