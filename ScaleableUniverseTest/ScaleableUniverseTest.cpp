#include "Matrix3Tests.h"
#include "Matrix3Tests.cpp"		// This is horse shit. If I don't include the .cpp, main tries to use functions without their defition.
#include "Network/NetworkTest.h"
//#include "Network/NetworkTest.cpp"
#include <assert.h>
#include <tchar.h>
#include <stdio.h>

#ifdef DEBUG
#define check assert
#else
#define check(n) if ( !(n) ) { printf( "check failed\n" ); exit(1); }
#endif

int _tmain(int argc, _TCHAR* argv[])
{
	// MATH TESTS
	bool identityTestSuccess = SAU::Matrix3Tests::identityTest();
	check(identityTestSuccess);
	bool inverseTestSuccess = SAU::Matrix3Tests::matrixInverseTest(100);
	check(inverseTestSuccess);

	// NETWORK TESTS
	if (!SAU::NetworkTest::initializeSockets())
	{
		printf("failed to initialize sockets\n");
		return 1;
	}
	check(SAU::NetworkTest::singleConnectionTest());
	check(SAU::NetworkTest::multiConnectionTest());
	check(SAU::NetworkTest::singlePacketTest());
	check(SAU::NetworkTest::doublePacketTest());


	return 0;
}


