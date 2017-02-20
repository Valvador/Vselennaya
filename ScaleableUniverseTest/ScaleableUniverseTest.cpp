#include "Matrix3Tests.h"
#include "Matrix3Tests.cpp"		// This is horse shit. If I don't include the .cpp, main tries to use functions without their defition.
#include <assert.h>
#include <tchar.h>


int _tmain(int argc, _TCHAR* argv[])
{
	bool identityTestSuccess = SAUTest::Matrix3Tests::identityTest();
	assert(identityTestSuccess);
	bool inverseTestSuccess = SAUTest::Matrix3Tests::matrixInverseTest(100);
	assert(inverseTestSuccess);
	return 0;
}


