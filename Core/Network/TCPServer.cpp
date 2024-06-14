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
			msg.append(std::to_wstring(25565));
			Logger::Fatal(msg);
			return Error;
		}

		if (listen(this->m_sock, SOMAXCONN) == -1)
		{
			String msg = "Oops, socket appears to be deaf, failed to listen to port ";
			msg.append(std::to_wstring(25565));
			Logger::Fatal(msg);
			return Error;
		}
		return Done;
	}
	void TCPServer::Close()
	{
		Socket::Close();
	}
	SocketStatus TCPServer::AcceptConnection(TCPSocket& sock)
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
}