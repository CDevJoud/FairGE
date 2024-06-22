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
		SOCKET GetNativeSocket() const;
	protected:
		SOCKET m_sock;

	private:
		Type m_type;
	};
}