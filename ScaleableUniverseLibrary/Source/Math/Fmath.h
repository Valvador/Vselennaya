#pragma once
#include <limits>

namespace SAU
{
	static bool fEquals(float a, float b, float epsilon = -1.0)
	{
		if (epsilon < 0)
		{
			if (std::numeric_limits<float>::epsilon() > std::abs(a - b))
				return true;
		}
		else
		{
			if (epsilon > std::abs(a - b))
			{
				return true;
			}
		}

		return false;
	}
}