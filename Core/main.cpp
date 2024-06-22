#pragma comment(lib, "Ws2_32.lib")
#include "Core/Server.hpp"
using namespace ugr::Core;

int main()
{
	Server FairGEServer;
	FairGEServer.Run();
}