#pragma once
#include "..\Definitions.hpp"
#include <Windows.h>

namespace ugr::Debugger
{
	class Logger
	{
	public:
		
		
		//static void InitLogger(String fileName);

		static void Msg(String msg);
		static void Info(String msg);
		static void Warn(String msg);
		static void Error(String msg);
		static void Fatal(String msg);

		
		
	private:
		enum class Level
		{
			Msg = 0, Info, Warning, Error, Fatal
		};

		static void Log(String& msg, Level lvl);
		static Logger GetInstance();

		static String GetTime();

		
	};
	
}