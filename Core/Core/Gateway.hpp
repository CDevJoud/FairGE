#pragma once
#include "..\Network\TCPServer.hpp"
#include"ClientHandler.hpp"
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
		std::vector<ClientHandler*> ClientHandlers;
		std::mutex mtx;
	};
}
