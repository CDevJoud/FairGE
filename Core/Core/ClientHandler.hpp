#pragma once
#include "..\Network\TCPSocket.hpp"

namespace ugr::Core
{
	class ClientHandler
	{
	public:
		ClientHandler(Network::TCPSocket* sock);

		bool IsDisconnected() const;
		void StartUpThread();
	private:
		static DWORD HandleClient(LPVOID);
		Network::TCPSocket* sock;
		HANDLE hThread = NULL;
		SocketStatus status;
	};
}
