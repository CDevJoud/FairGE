#pragma once
#include "..\Utility.hpp"
#include "..\Debugger\Logger.hpp"
#include <vector>
#define HANDSHACKE_LAYOUT_FORMAT "Int32 Int32 RString Int16 Int32"
#define LOGINSTART_LAYOUT_FORMAT "Int32 RString Int32"

namespace ugr::Network
{
	using namespace Debugger;
	class Packet
	{
		//Packets Structures
	public:
#pragma pack(push, 1)
		struct LoginStart
		{
			Int32 ID;
			RString Name;
			Int32 UUID;
		};
#pragma pack(pop)
		struct HandShacke
		{
			Int32 ID, ProtocolVersionNumber;
			RString IP;
			Int32 UnusedPort, NextState;
		};
	public:
		Packet(Uint64 buffersize);
		Packet() = default;
		Packet(AString str);

		virtual ~Packet() = default;

		void Append(const void* data, Uint64 size);

		Uint64 GetReadPosition() const;

		void Clear();
		const void* GetBufferData() const;

		Uint64 GetBufferSize() const;
		Uint64 GetPacketSize() const;
		bool EndOfPacket() const;

		void SetBufferDataSize(Uint64 size);

		explicit operator bool() const;
		
		/*Packet& operator>>(VarInt32& data);
		Packet& operator<<(VarInt32 data);*/

		Packet& operator>>(Int32& data);
		Packet& operator<<(Int32 data);

		Packet& operator>>(AString& data);


		void Push(Int32 val);
		void Push(AString str);

		void UnpackWithFormat(void* data, AString format);

		void Build();
		Int32 GetInt32(Int32 bytes = 0);
		AString GetAString();
	private:
		bool CheckSize(Uint64 size);
		std::vector<Byte> data;
		Uint64 readPos = 0, sendPos = 0, packsize = 0;
		bool isValid = true;
	};
	inline Int32 Packet::GetInt32(Int32 bytes)
	{
		if(bytes == 0)
		{
			if (this->data.size() < 5)
			{
				std::vector<Byte> tmpb(5);
				tmpb.insert(tmpb.begin(), this->data.begin() + this->readPos, this->data.end());
				Uint64 in = 0;
				Int32 val = DecodeVarInt32(*reinterpret_cast<VarInt32*>(tmpb.data()), in);
				this->readPos += in;
				return val;
			}
			else
			{
				Uint64 in = 0;
				Int32 val = DecodeVarInt32(*reinterpret_cast<VarInt32*>(this->data.data() + this->readPos), in);
				this->readPos += in;
				return val;
			}
		}
		else
		{
			VarInt32 tmp{};
			switch (bytes)
			{
			case 0:
				break;
			case 1:
			{
				tmp.b1 = this->data.data()[this->readPos];
				Uint64 in = 0;
				Int32 val = DecodeVarInt32(tmp, in);
				this->readPos += in;
				return val;
			}
			case 2:
			{
				tmp.b1 = this->data.data()[this->readPos++];
				tmp.b2 = this->data.data()[this->readPos];
				Uint64 in = 0;
				Int32 val = DecodeVarInt32(tmp, in);
				this->readPos += in;
				return val;
			}
			default:
				Logger::Error("The bytes You have specified is out of range!");
				break;
			}
		}
	}
	inline AString Packet::GetAString()
	{
		Uint64 in = 0;
		Int32 strsize = DecodeVarInt32(*reinterpret_cast<VarInt32*>(this->data.data() + this->readPos), in);
		this->readPos += in;
		AString str;
		for (int i = this->readPos; i < strsize + this->readPos; i++)
			str.push_back(this->data[i]);
		this->readPos += strsize;
		return str;
	}
}