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
			return (this->status == Disconnected);
	}

	void ClientHandler::StartUpThread()
	{
		this->hThread = CreateThread(NULL, NULL, HandleClient, this, NULL, NULL);
	}

	DWORD ClientHandler::HandleClient(LPVOID lpParam)
	{
		ClientHandler* ch = reinterpret_cast<ClientHandler*>(lpParam);
		AString buffer;
		buffer.resize(64);
		Uint64 r;
		ch->status = ch->sock->Receive(buffer.data(), buffer.size(), r);
		Network::Packet p = buffer;
		while(ch->loop)
		{
			struct {
				Int32 PacketID;
				Int32 ProVersion;
				AString ServerAddress;
				Int32 IgnoredPort0;
				Int32 IgnoredPort1;
				Int32 State;
			}HandShackePacket{};
			p.UnpackWithFormat(&HandShackePacket, "Int32 Int32 AString Int32 Int32 Int32");
			AString jsonresponse = R"({
    "version": {
        "name": "1.19.4",
        "protocol": 762
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
}
)";

			AString packet;
			packet.push_back(0xD6);
			packet.push_back(0x03);
			packet.push_back(0x00);
			packet.push_back(0xD5);
			packet.push_back(0x03);
			packet.append(jsonresponse);

			ch->status = ch->sock->Send(packet.data(), packet.size());
			if (ch->status != Done)
				break;
		}
		Logger::Msg("Client Disconnected!");
		ch->status = Disconnected;
		ch->sock->Disconnect();
		return 0;
	}
	
}
