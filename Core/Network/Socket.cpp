#include "Socket.hpp"
#include "..\Debugger\Logger.hpp"

namespace ugr::Network
{
	using namespace Debugger;
	sockaddr_in SocketImplementation::CreateAddress(AString address, Uint16 port)
	{
		sockaddr_in tmp{};
		tmp.sin_family = AF_INET;
		tmp.sin_port = htons(port);
		inet_pton(AF_INET, address.c_str(), &tmp.sin_addr);
		return tmp;
	}
	void SocketImplementation::Close(SOCKET sock)
	{
		closesocket(sock);
	}
	SocketStatus SocketImplementation::GetErrorStatus()
	{
		switch (WSAGetLastError())
		{
		case WSAEWOULDBLOCK:  return SocketStatus::NotReady;
		case WSAEALREADY:     return SocketStatus::NotReady;
		case WSAECONNABORTED: return SocketStatus::Disconnected;
		case WSAECONNRESET:   return SocketStatus::Disconnected;
		case WSAETIMEDOUT:    return SocketStatus::Disconnected;
		case WSAENETRESET:    return SocketStatus::Disconnected;
		case WSAENOTCONN:     return SocketStatus::Disconnected;
		case WSAEISCONN:      return SocketStatus::Done; // when connecting a non-blocking socket
		default:              return SocketStatus::Error;
		}
	}

	struct WindowsSocketInit
	{
		WindowsSocketInit()
		{
			Logger::Info("FairGE-Core is starting up with the version of b0.0.1");
			Logger::Info("This version of FairGE-Core-Server Support Minecraft Versions 1.8 - 1.20.6!");
			WSAData init;
			Logger::Info("Init Windows Socket API For Networking Purposes!");
			WSAStartup(MAKEWORD(2, 2), &init);
			Logger::Info("Done!");
		}
		~WindowsSocketInit()
		{
			Logger::Info("Shuting Down The Windows Socket API!");
			WSACleanup();
			Logger::Info("Done!");
		}
	};
	WindowsSocketInit gInit;
	void Socket::Create()
	{
		if (this->m_sock == INVALID_SOCKET)
		{
			const SOCKET sock = socket(AF_INET, this->m_type == Type::TCP ? SOCK_STREAM : SOCK_DGRAM, 0);
			if (sock == INVALID_SOCKET)
			{
				Logger::Error("ugr::Network::Socket::Create() -> Failed to create socket!");
				//return;
			}

			this->Create(sock);
		}
	}
	void Socket::Create(SOCKET sock)
	{
		if (this->m_sock == INVALID_SOCKET)
		{
			this->m_sock = sock;
			if (this->m_type == Type::TCP)
			{
				int yes = 1;
				if (setsockopt(this->m_sock, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<char*>(&yes), sizeof(yes)) == 1)
				{
					Logger::Error("ugr::Network::Socket::Create(SOCKET) -> Failed to Disable Nagle algorithm");
				}
			}
		}
	}
	Socket::Socket(Type type) : m_type(type), m_sock(INVALID_SOCKET)
	{

	}
	void Socket::Close()
	{
		if (this->m_sock != INVALID_SOCKET)
		{
			SocketImplementation::Close(this->m_sock);
			this->m_sock = INVALID_SOCKET;
		}
	}
}
