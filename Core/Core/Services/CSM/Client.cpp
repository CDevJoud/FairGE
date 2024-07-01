#include "Client.hpp"
namespace ugr::Core::CSM
{
	Client::Client(Network::TCPSocket* sock) : Network::TCPSocket(*sock)
	{

	}
	void Client::ProcessRun()
	{
		String buffer;
		buffer.resize(128);
		Uint64 r;
		this->Receive(buffer.data(), buffer.size(), r);

		Int32 usernamesize = buffer[0];
		String username;
		username.insert(username.begin(), buffer.begin() + 1, buffer.begin() + 1 + usernamesize);
		String password;
		Int32 passwordsize = buffer.data()[1 + usernamesize];
		password.insert(password.begin(), buffer.begin() + 2 + usernamesize, buffer.begin() + passwordsize + usernamesize + 2);
		Debugger::Logger::Msg(buffer.c_str());

		this->Send(L"Correct!", 24);
		this->Disconnect();

		Debugger::Logger::Msg("client Disconnected!");
	}
}
