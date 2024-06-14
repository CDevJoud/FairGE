#include "Gateway.hpp"
#include <vector>
#include "ClientHandler.hpp"
#include "..\Debugger\Logger.hpp"


namespace ugr::Core
{
	using namespace Debugger;
	void Gateway::Run()
	{
		this->ListenOn("192.168.1.110", 25565);
		
		this->hThread = CreateThread(NULL, NULL, Gateway::CheckIfClientDisconnected, this, NULL, NULL);
		while (true)
		{
			Network::TCPSocket* sock = new Network::TCPSocket;
			if (this->AcceptConnection(*sock) != Done)
			{
				delete sock;
				continue;
			}
			Logger::Msg("New Client Connected");
			std::shared_ptr<ClientHandler> handler = std::make_shared<ClientHandler>(sock);
			{
				std::lock_guard<std::mutex> lock(mtx);
				ClientHandlers.push_back(handler);
			}
			handler->StartUpThread();
		}
	}
	DWORD __stdcall Gateway::CheckIfClientDisconnected(LPVOID lpParam)
	{
		Gateway* gy = reinterpret_cast<Gateway*>(lpParam);
		while (true)
		{
			std::lock_guard<std::mutex> lock(gy->mtx);
			for (auto it = gy->ClientHandlers.begin(); it != gy->ClientHandlers.end(); ) 
				if ((*it)->IsDisconnected()) 
					it = gy->ClientHandlers.erase(it); 
				else
					++it;
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
		return 0;
	}
}