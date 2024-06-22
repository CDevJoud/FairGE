#pragma once
#include "TCPSocket.hpp"

namespace ugr::Network
{
	class TCPServer : public Socket
	{
	public:
		TCPServer();

		SocketStatus ListenOn(const AString& IpAddress, Uint16 port);
		void Close();

		SocketStatus AcceptConnection(TCPSocket& sock, float time = 0.0f);
	};
}
