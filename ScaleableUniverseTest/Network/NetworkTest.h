#ifndef NetTestH
#define NetTestH

namespace SAU
{
	class NetworkTest
	{
	private:
	public:
		static bool initializeSockets();
		static bool singleConnectionTest();
		static bool multiConnectionTest();
	};
}

#endif