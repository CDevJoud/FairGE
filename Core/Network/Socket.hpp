#pragma once
#include <WS2tcpip.h>
#include "..\Definitions.hpp"

namespace ugr::Network
{
	class SocketImplementation
	{
	public:
		static sockaddr_in CreateAddress(AString address, Uint16 port);

		static void Close(SOCKET sock);

		static SocketStatus GetErrorStatus();

	};
	class Socket
	{
	public:
		enum class Type
		{
			TCP, UDP
		};
		Socket(Type type);
		void Close();
		void Create();
		void Create(SOCKET sock);
		SOCKET m_sock;
	protected:
		

	private:
		Type m_type;
	};
}