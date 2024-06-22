#include "Gateway.hpp"
#include <vector>
//#include "ClientHandler.hpp"
#include "..\Debugger\Logger.hpp"

namespace ugr::Core
{
	using namespace Debugger;
	void Gateway::Run()
	{
		this->ListenOn("192.168.1.110", 25565);
		
		this->hThread = CreateThread(NULL, NULL, Gateway::CheckIfClientDisconnected, this, NULL, NULL);
		while (Properties::Server::Loop)
		{
			Network::TCPSocket* sock = new Network::TCPSocket;
			if (this->AcceptConnection(*sock) != Done)
			{
				delete sock;
				continue;
			}
			if (!Properties::Server::Loop)
			{
				delete sock;
				break;
			}
			Logger::Msg("New Client Connected");
			ClientVersionResolver* handler = new ClientVersionResolver(sock);
			{
				std::lock_guard<std::mutex> lock(mtx);
				this->VersionResolvers.push_back(handler);
			}
			handler->ResolveVersion();
		}
	}
	DWORD __stdcall Gateway::CheckIfClientDisconnected(LPVOID lpParam)
	{
		Gateway* gy = reinterpret_cast<Gateway*>(lpParam);
		while (Properties::Server::Loop || !gy->VersionResolvers.empty())
		{
			std::lock_guard<std::mutex> lock(gy->mtx);
			for (auto it = gy->VersionResolvers.begin(); it != gy->VersionResolvers.end(); )
				if ((*it)->IsDisconnected()) 
				{
					(*it)->WaitTillResolvingEnds();
					delete* it;
					it = gy->VersionResolvers.erase(it);
				}
				else
					++it;
			Sleep(10);
		}
			
		return 0;
	}
}