#pragma once
#include "..\..\..\Network\TCPSocket.hpp"

namespace ugr::Core::CSM
{
	class Client : public Network::TCPSocket
	{
	public:
		Client(Network::TCPSocket* sock);

		void ProcessRun();
	};
}
