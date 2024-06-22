#pragma once
#include "Definitions.hpp"

namespace ugr::Core
{
	class Properties
	{
	public:
		struct Server
		{
			static AString b64ServerIcon;
			static Uint64 PlayersOnline;
			static Uint64 PlayersLimit;

			static bool Loop;
		};
	};
	
}