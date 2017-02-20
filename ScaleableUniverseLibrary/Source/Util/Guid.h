#pragma once

namespace SAU
{
	class GuidGenerator
	{
	private:
		static int lastUsed;

	public:
		static void initGuidGen();
		static int generateGuid();
	};
};