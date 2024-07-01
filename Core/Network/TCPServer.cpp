#include "TCPServer.hpp"
#include "..\Debugger\Logger.hpp"
namespace ugr::Network
{
	using namespace Debugger;
	TCPServer::TCPServer() : Socket(Type::TCP)
	{

	}
	SocketStatus TCPServer::ListenOn(const AString& IpAddress, Uint16 port)
	{
		this->Close();

		this->Create();

		if (IpAddress == "255.255.255.255")
			return Error;

		sockaddr_in addr = SocketImplementation::CreateAddress(IpAddress, port);
		if (bind(this->m_sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == -1)
		{
			String msg = "Failed to bind to port ";
			msg.append(std::to_wstring(port));
			Logger::Fatal(msg);
			return Error;
		}

		if (listen(this->m_sock, SOMAXCONN) == -1)
		{
			String msg = "Oops, socket appears to be deaf, failed to listen to port ";
			msg.append(std::to_wstring(port));
			Logger::Fatal(msg);
			return Error;
		}
		return Done;
	}
	void TCPServer::Close()
	{
		Socket::Close();
	}
	SocketStatus TCPServer::AcceptConnection(TCPSocket& sock, float time)
	{
		if(time == 0)
		{
			if (this->m_sock == INVALID_SOCKET)
			{
				Logger::Error("Failed to accept a new connection, the socket appears to be deaf :(");
				return Error;
			}
			sockaddr_in address{};
			Int32 length = sizeof(address);
			const SOCKET remote = accept(this->m_sock, reinterpret_cast<sockaddr*>(&address), &length);

			if (remote == INVALID_SOCKET)
				return SocketImplementation::GetErrorStatus();

			sock.Close();
			sock.Create(remote);

			return Done;
		}

		fd_set fds;
		struct timeval tv;

		FD_ZERO(&fds);
		FD_SET(this->m_sock, &fds);
		tv.tv_sec = 0;
		tv.tv_usec = time;
		select(0, &fds, NULL, NULL, &tv);
		if(this->m_sock == INVALID_SOCKET)
		{
			Logger::Error("Failed to accept a new connection, the socket appears to be deaf :(");
			return Error;
		}
		sockaddr_in address{};
		Int32 length = sizeof(address);
		if (FD_ISSET(this->m_sock, &fds))
		{
			const SOCKET remote = accept(this->m_sock, reinterpret_cast<sockaddr*>(&address), &length);
			if (remote == INVALID_SOCKET)
				return SocketImplementation::GetErrorStatus();
			sock.Close();
			sock.Create(remote);
		}
		return Done;
		
	}
}