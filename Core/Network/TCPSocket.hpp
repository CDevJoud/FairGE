#pragma once
#include "Socket.hpp"
#include "Packet.hpp"
namespace ugr::Network
{
	class TCPSocket : public Socket
	{
	public:
		TCPSocket();

		SocketStatus Connect(const String& IpAddress, Uint16 port);

		void Disconnect();

		SocketStatus Send(const void* data, Uint64 size);
		SocketStatus Send(const void* data, Uint64 size, Uint64& sent);

		SocketStatus Receive(void* data, Uint64 size, Uint64& received);

		SocketStatus Send(Packet& p);

		SocketStatus Receive(Packet& p);
	};
}
