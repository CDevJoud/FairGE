#include "Server.hpp"
#include "..\Utility.hpp"

namespace ugr::Core
{
	HANDLE Server::hMCThread;
	HANDLE Server::hCSMThread;
	Gateway* Server::MMCGateway = new Gateway;
	CSM::GGateway* Server::CSMGateway = new CSM::GGateway;
	
	DWORD __stdcall MMCServerThread(LPVOID lpParam)
	{
		Core::Gateway* gy = reinterpret_cast<Core::Gateway*>(lpParam);
		gy->Run();
		return 0;
	}
	static DWORD __stdcall CSMServerThread(LPVOID lpParam)
	{
		CSM::GGateway* gy = reinterpret_cast<CSM::GGateway*>(lpParam);
		gy->Start();
		return 0;
	}
	Server::Server()
	{
		this->InitServerProperties();
		this->InitMinecraftServer();
		this->InitCSMServer();
		this->InitHTTPServer();
	}
	Server::~Server()
	{
		delete this->MMCGateway;
		CloseHandle(this->hMCThread);
	}
	void Server::InitServerProperties()
	{
		HANDLE hFile = CreateFileA("server-icon.png", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		LARGE_INTEGER size;
		GetFileSizeEx(hFile, &size);
		AString buffer;
		buffer.resize(size.QuadPart);
		DWORD r = 0;
		if (hFile != INVALID_HANDLE_VALUE)
		{
			ReadFile(hFile, buffer.data(), size.QuadPart, &r, NULL);
			Core::Properties::Server::b64ServerIcon += "data:image/png;base64,";
			AString b64Img = EncodeBase64(buffer);
			Core::Properties::Server::b64ServerIcon.insert(Core::Properties::Server::b64ServerIcon.end(), b64Img.begin(), b64Img.end());
			Core::Properties::Server::PlayersLimit = 100;
			Core::Properties::Server::PlayersOnline = 0;
		}
		CloseHandle(hFile);
		Properties::Server::Loop = true;
	}
	void Server::InitMinecraftServer()
	{

	}
	void Server::InitCSMServer()
	{

	}
	void Server::InitHTTPServer()
	{

	}
	void Server::Run()
	{
		hCSMThread = CreateThread(NULL, NULL, CSMServerThread, CSMGateway, NULL, NULL);
		hMCThread = CreateThread(NULL, NULL, MMCServerThread, MMCGateway, NULL, NULL);
		WaitForSingleObject(hCSMThread, INFINITE);
		WaitForSingleObject(hMCThread, INFINITE);
		
	}
	void Server::Stop()
	{
		Properties::Server::Loop = false;
	}
}
