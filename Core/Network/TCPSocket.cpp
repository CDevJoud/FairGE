#include "TCPSocket.hpp"
#include "..\Debugger\Logger.hpp"

namespace ugr::Network
{
	using namespace Debugger;
	TCPSocket::TCPSocket() : Socket(Type::TCP)
	{

	}
	SocketStatus TCPSocket::Connect(const String& IpAddress, Uint16 port)
	{
		this->Disconnect();

		this->Create();
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		AString hostname = converter.to_bytes(IpAddress);
		addrinfo hints = {};
		addrinfo* result = nullptr;

		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		Int32 addrInfoResult = getaddrinfo(hostname.c_str(), nullptr, &hints, &result);
		if (addrInfoResult != 0)
		{
			Logger::Error("getaddrinfo failed: " + String(gai_strerrorA(addrInfoResult)));
		}
		addrinfo* ptr = result;
		AString IpString;
		for (; ptr != nullptr; ptr = ptr->ai_next) {
			void* addr;
			char ipstr[INET6_ADDRSTRLEN];

			if (ptr->ai_family == AF_INET) { // IPv4
				struct sockaddr_in* ipv4 = (struct sockaddr_in*)ptr->ai_addr;
				addr = &(ipv4->sin_addr);
			}
			else if (ptr->ai_family == AF_INET6) { // IPv6
				struct sockaddr_in6* ipv6 = (struct sockaddr_in6*)ptr->ai_addr;
				addr = &(ipv6->sin6_addr);
			}
			else {
				continue;
			}

			// Convert the IP to a string
			inet_ntop(ptr->ai_family, addr, ipstr, sizeof(ipstr));
			IpString = ipstr;
		}

		sockaddr_in address = SocketImplementation::CreateAddress(IpString, port);

		if (connect(this->m_sock, reinterpret_cast<PSOCKADDR>(&address), sizeof(address)) == -1)
			return SocketImplementation::GetErrorStatus();

		return SocketStatus::Done;
	}
	void TCPSocket::Disconnect()
	{
		this->Close();
	}
	SocketStatus TCPSocket::Send(const void* data, Uint64 size)
	{
		Uint64 sent = 0;
		return Send(data, size, sent);
	}
	SocketStatus TCPSocket::Send(const void* data, Uint64 size, Uint64& sent)
	{
		if (!data || (size == 0))
		{
			Logger::Warn("TCPSocket::Send(const void* data, Uint64 size, Uint64& sent) -> Cannot send data over the network (no data to send)");
			return SocketStatus::Error;
		}
		if (this->m_sock == INVALID_SOCKET)
		{
			Logger::Error("TCPSocket::Send(const void* data, Uint64 size, Uint64& sent) -> Cannot send data, the socket appears to be Invalid!");
			return Error;
		}
		int result = 0;
		for (sent = 0; sent < size; sent += static_cast<Uint64>(result))
		{
			result = static_cast<Int32>(send(this->m_sock, static_cast<const char*>(data) + sent, static_cast<Int32>(size - sent), 0));
			if (result < 0)
			{
				const SocketStatus Status = SocketImplementation::GetErrorStatus();
				if ((Status == SocketStatus::NotReady) && sent)
					return SocketStatus::Partial;

				return Status;
			}
		}
		return SocketStatus::Done;
	}
	SocketStatus TCPSocket::Receive(void* data, Uint64 size, Uint64& received)
	{
		received = 0;

		// Check the destination buffer
		if (!data)
		{
			Logger::Error("ugr::Network::TCPSocket::Receive(void* data, Uint64 size, Uint64& received) -> Cannot receive data from the network (the destination buffer is invalid)");
			return SocketStatus::Error;
		}

		// Receive a chunk of bytes
		const int sizeReceived = static_cast<int>(
			recv(this->m_sock, static_cast<char*>(data), static_cast<Int32>(size), 0));

		// Check the number of bytes received
		if (sizeReceived > 0)
		{
			received = static_cast<std::size_t>(sizeReceived);
			return SocketStatus::Done;
		}
		else if (sizeReceived == 0)
		{
			return SocketStatus::Disconnected;
		}
		else
		{
			return SocketImplementation::GetErrorStatus();
		}
	}
}
