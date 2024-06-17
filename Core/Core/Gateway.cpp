#include "Gateway.hpp"
#include <vector>
#include "ClientHandler.hpp"
#include "..\Debugger\Logger.hpp"


namespace ugr::Core
{
	using namespace Debugger;
	void Gateway::Run()
	{
		this->ListenOn("192.168.0.134", 25565);
		
		//this->hThread = CreateThread(NULL, NULL, Gateway::CheckIfClientDisconnected, this, NULL, NULL);
		while (true)
		{
			Network::TCPSocket* sock = new Network::TCPSocket;
			if (this->AcceptConnection(*sock) != Done)
			{
				delete sock;
				continue;
			}
			Logger::Msg("New Client Connected");
			ClientHandler* handler = new ClientHandler(sock);
			{
				std::lock_guard<std::mutex> lock(mtx);
				ClientHandlers.push_back(handler);
			}

			//handler->StartUpThread();
		}
	}
	DWORD __stdcall Gateway::CheckIfClientDisconnected(LPVOID lpParam)
	{
		Gateway* gy = reinterpret_cast<Gateway*>(lpParam);
		while (true)
		{
			std::lock_guard<std::mutex> lock(gy->mtx);
			for (auto it = gy->ClientHandlers.begin(); it != gy->ClientHandlers.end(); ) 
			{
				if ((*it)->IsDisconnected()) 
				{
					// Clean up the handler
					(*it)->loop = false;
					delete* it;
					it = gy->ClientHandlers.erase(it);
				}
				else {
					++it;
				}
			}
		}
			Sleep(10);
		return 0;
	}
}