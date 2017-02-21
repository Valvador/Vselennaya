#include "Matrix3Tests.h"
#include "Matrix3Tests.cpp"		// This is horse shit. If I don't include the .cpp, main tries to use functions without their defition.
#include "Network/NetworkTest.h"
#include <assert.h>
#include <tchar.h>

#ifdef DEBUG
#define check assert
#else
#define check(n) if ( !(n) ) { printf( "check failed\n" ); exit(1); }
#endif

int _tmain(int argc, _TCHAR* argv[])
{
	// MATH TESTS
	bool identityTestSuccess = SAUTest::Matrix3Tests::identityTest();
	check(identityTestSuccess);
	bool inverseTestSuccess = SAUTest::Matrix3Tests::matrixInverseTest(100);
	check(inverseTestSuccess);

	// NETWORK TESTS
	if (!SAU::NetworkTest::initializeSockets())
	{
		printf("failed to initialize sockets\n");
		return 1;
	}
	check(SAU::NetworkTest::singleConnectionTest());
	check(SAU::NetworkTest::multiConnectionTest(6));



	return 0;
}


