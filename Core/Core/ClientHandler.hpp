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
		SocketStatus status;
		HANDLE hThread = NULL;
	private:
		//static DWORD HandleClient(LPVOID);
		Network::TCPSocket* sock;
		
		
	};
}
