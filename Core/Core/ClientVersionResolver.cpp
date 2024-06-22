#include "ClientVersionResolver.hpp"
#include "..\Network\Socket.hpp"
#include "..\json.hpp"
#include "..\Debugger\Logger.hpp"
#include "..\Core\Server.hpp"

namespace ugr::Core
{
	using namespace ugr::Debugger;
	static DWORD APIENTRY ResolveVersion(LPVOID lpParam)
	{
		ClientVersionResolver* vr = (ClientVersionResolver*)lpParam;
		Network::TCPSocket* c = vr->GetClient();
		Network::Packet::HandShacke hs{};
		Network::Packet::LoginStart ls{};
		Network::Packet p(128);
		nlohmann::json data;
		if (!vr->IsDisconnected())
		{
			c->Receive(p);
			p.UnpackWithFormat(&hs, HANDSHACKE_LAYOUT_FORMAT);
			if (hs.ProtocolVersionNumber == 47 && hs.NextState == 1)
			{
				p.Clear();
				p.Push(0);
				data["version"]["name"] = "1.8.9";
				data["version"]["protocol"] = 47;
				data["players"]["max"] = Properties::Server::PlayersLimit;
				data["players"]["online"] = Properties::Server::PlayersOnline++;
				data["description"]["text"] = "This is a Server That runs on C++!";
				data["favicon"] = Properties::Server::b64ServerIcon;
				data["enforcesSecureChat"] = false;
				data["previewsChat"] = false;
				p.Push(data.dump(0));

				p.Build();
				c->Send(p);
				p.Clear();
				p.SetBufferDataSize(32);
				c->Receive(p);
				c->Send(p);
				c->Disconnect();
			}
			if (hs.NextState == 2)
			{
				p.UnpackWithFormat(&ls, LOGINSTART_LAYOUT_FORMAT);
				c->Disconnect();
			}
		}
		Logger::Msg("Client Disconnected!");
		return 0;
	}
	ClientVersionResolver::ClientVersionResolver(Network::TCPSocket* sock)
	{
		this->status = Done;
		this->client = sock;
	}
	ClientVersionResolver::~ClientVersionResolver()
	{
		delete this->client;
	}
	void ClientVersionResolver::ResolveVersion()
	{
		this->hThread = CreateThread(NULL, NULL, Core::ResolveVersion, this, NULL, NULL);
	}
	bool ClientVersionResolver::IsDisconnected() const
	{
		return (this->status == Disconnected || this->client->GetNativeSocket() == INVALID_SOCKET);
	}
	void ClientVersionResolver::WaitTillResolvingEnds() const
	{
		WaitForSingleObject(this->hThread, INFINITE);
	}
	Network::TCPSocket* ClientVersionResolver::GetClient() const
	{
		return this->client;
	}
}
