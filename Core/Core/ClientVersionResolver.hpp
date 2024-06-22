#pragma once
#include "..\Network\TCPSocket.hpp"

namespace ugr::Core
{
	class ClientVersionResolver
	{
	public:
		ClientVersionResolver(Network::TCPSocket* sock);
		~ClientVersionResolver();

		void ResolveVersion();
		bool IsDisconnected() const;
		void WaitTillResolvingEnds() const;
		Network::TCPSocket* GetClient() const;
	private:
		Network::TCPSocket* client;
		SocketStatus status;
		HANDLE hThread = NULL;
	};
}