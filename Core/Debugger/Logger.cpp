#include "Logger.hpp"
#include <Windows.h>
namespace ugr::Debugger
{

	Logger Logger::GetInstance()
	{
		static Logger Instance;
		return Instance;
	}
	void Logger::Msg(String msg)
	{
		GetInstance().Log(msg, Level::Msg);
	}
	void Logger::Info(String msg)
	{
		GetInstance().Log(msg, Level::Info);
	}
	void Logger::Warn(String msg)
	{
		GetInstance().Log(msg, Level::Warning);
	}
	void Logger::Error(String msg)
	{
		GetInstance().Log(msg, Level::Error);
	}
	void Logger::Fatal(String msg)
	{
		GetInstance().Log(msg, Level::Fatal);
	}
	String Logger::GetTime()
	{
		SYSTEMTIME Time{};
		GetLocalTime(&Time);
		String str;
		str += L"[" + std::to_wstring(Time.wYear) + L"-" + std::to_wstring(Time.wMonth) + L"-" + std::to_wstring(Time.wDay) + L" " + std::to_wstring(Time.wHour) + L":" + std::to_wstring(Time.wMinute) + L':' + std::to_wstring(Time.wSecond) + L"] ";
		return str;
		
	}
	void Logger::Log(String& msg, Level lvl)
	{
		switch (lvl)
		{
		case Logger::Level::Msg:
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
			break;
		case Logger::Level::Info:
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x09);
			break;
		case Logger::Level::Warning:
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x06);
			break;
		case Logger::Level::Error:
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x04);
			break;
		case Logger::Level::Fatal:
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x40);
			break;
		default:
			break;
		}
		DWORD w;
		String strTime = GetInstance().GetTime();

		WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), strTime.c_str(), strTime.size(), &w, NULL);
		WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), msg.c_str(), msg.size(), &w, NULL);
		WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), "\n", 1, &w, NULL);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
	}
	Logger gLogger;
}
