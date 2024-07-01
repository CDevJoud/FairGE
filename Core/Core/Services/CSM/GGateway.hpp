#pragma once
#include "..\..\..\Network\TCPServer.hpp"
#include "Client.hpp"
#include <vector>

namespace ugr::Core::CSM
{
	class GGateway : Network::TCPServer
	{
	public:
		GGateway() = default;

		void Start();
	private:
		std::vector<Client*> clients;
	};
}
