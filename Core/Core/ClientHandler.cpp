#include "ClientHandler.hpp"
#include "..\Debugger\Logger.hpp"
#include "..\Network\Packet.hpp"

namespace ugr::Core
{
	using namespace Debugger;
	ClientHandler::ClientHandler(Network::TCPSocket* sock) : status(Done)
	{
		this->sock = sock;
	}

	bool ClientHandler::IsDisconnected() const
	{
		if(this)
			return (this->sock->GetNativeSocket() == INVALID_SOCKET);
	}

	void ClientHandler::StartUpThread()
	{
		//this->hThread = CreateThread(NULL, NULL, HandleClient, this, NULL, NULL);
	}

	/*DWORD ClientHandler::HandleClient(LPVOID lpParam)
	{
		ClientHandler* ch = reinterpret_cast<ClientHandler*>(lpParam);
		do
		{
			Network::Packet packet;
			struct {
				Int32 ID;
				Int32 ProVersion;
				RString Ip;
				Int32 Port;
				Int32 State;
			}HandShacke{};
			AString buffer;
			buffer.resize(32);
			Uint64 r = 0;
			ch->sock->Receive(buffer.data(), buffer.size(), r);
			packet = buffer;
			packet.UnpackWithFormat(&HandShacke, "Int32 Int32 RString Int16 Int32");

			if (HandShacke.State == 2)
			{
				ch->sock->Disconnect();
				ch->status = Disconnected;
			}
			packet.Clear();
			buffer.clear();

			packet.Push(0);
			packet.Push(R"({
    "version": {
        "name": "1.19.4",
        "protocol": 47
    },
    "players": {
        "max": 100,
        "online": 5,
        "sample": [
            {
                "name": "thinkofdeath",
                "id": "4566e69f-c907-48ee-8d71-d7ba5aa00d20"
            }
        ]
    },
    "description": {
        "text": "Hello, world!"
    },
    "favicon": "data:image/png;base64,<data>",
    "enforcesSecureChat": false,
    "previewsChat": false
})");
			packet.Build();

			ch->sock->Send(packet.GetData(), packet.GetDataSize());
			buffer.clear();
			if (HandShacke.State == 1)
			{
				ch->status = Disconnected;
				ch->sock->Disconnect();
			}
		} while (!ch->IsDisconnected());
		Logger::Msg("Client Disconnected!");
		return 0;
	}*/
	/*DWORD WINAPI ClientHandler::HandleClient(LPVOID lpParam)
	{
		ClientHandler* ch = reinterpret_cast<ClientHandler*>(lpParam);
		auto& stat = ch->status;
		struct
		{
			Int32 ID, ProVersion;
			RString Ip;
			Int32 Port, State;
		}HandShacke;
		do
		{
			Network::Packet p(128);
			stat = ch->sock->Receive(p);
			p.UnpackWithFormat(&HandShacke, "Int32 Int32 RString Int16 Int32");

		} while (!ch->IsDisconnected());
		Logger::Msg("Client Disconnected!");
		return 250;
	}*/
}
