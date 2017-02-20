#include <vector>

namespace SAU
{
	class Actor;

	class Simulator
	{
	public:
		enum SimType
		{
			Default = 0
		};

		virtual void stepSimulator(const std::vector<Actor*>& actors) = 0;
	};
}