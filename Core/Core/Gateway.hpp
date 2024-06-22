#pragma once
#include "..\Network\TCPServer.hpp"
#include "ClientVersionResolver.hpp"
#include "..\Properties.hpp"
#include <vector>
#include <mutex>

namespace ugr::Core
{
	class Gateway : public Network::TCPServer
	{
	public:
		Gateway() = default;

		void Run();
	private:
		static DWORD WINAPI CheckIfClientDisconnected(LPVOID lpParam);
		HANDLE hThread;
		friend class Server;
		//std::vector<ClientHandler*> ClientHandlers;
		std::vector<ClientVersionResolver*> VersionResolvers;
		std::mutex mtx;
	};
}
