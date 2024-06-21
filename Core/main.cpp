#pragma comment(lib, "Ws2_32.lib")
#include "Definitions.hpp"
#include "Network/TCPServer.hpp"
#include "Debugger/Logger.hpp"
#include <Windows.h>
#include "Core/Gateway.hpp"
using namespace ugr;
using namespace ugr::Debugger;

int main()
{
	//This is ClientResolver Branch
	Core::Gateway gw;
	gw.Run();
}