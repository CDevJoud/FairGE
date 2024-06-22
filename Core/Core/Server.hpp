#pragma once
#include "Gateway.hpp"

namespace ugr::Core
{
	class Server
	{
	public:
		Server();
		~Server();

		static Server GetInstance()
		{
			static Server s;
			return s;
		}

		static void Run();
		static void Stop();
	private:
		friend class Gateway;
		void InitServerProperties();
		void InitMinecraftServer();
		void InitCSMServer();
		void InitHTTPServer();
		static Gateway* MCGateway;
		static HANDLE hMCThread;
	};
}
