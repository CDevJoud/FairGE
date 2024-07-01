#include "GGateway.hpp"
#include "Client.hpp"
#include "..\..\..\Debugger\Logger.hpp"

namespace ugr::Core::CSM
{
	void GGateway::Start()
	{
		this->ListenOn("192.168.1.110", 25560);

		while (true)
		{
			Network::TCPSocket* TCPClient = new Network::TCPSocket;
			if (this->AcceptConnection(*TCPClient) != Done)
			{
				delete TCPClient;
				continue;
			}
			Debugger::Logger::Msg("New Client Connected CSM!");
			Client* client = new Client(TCPClient);
			this->clients.push_back(client);
			client->ProcessRun();
		}
	}
}