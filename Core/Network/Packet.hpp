#pragma once
#include "..\Utility.hpp"
#include "..\Debugger\Logger.hpp"
#include <vector>


namespace ugr::Network
{
	using namespace Debugger;
	class Packet
	{
	public:
		Packet() = default;
		Packet(AString str);

		virtual ~Packet() = default;

		void Append(const void* data, Uint64 size);

		Uint64 GetReadPosition() const;

		void Clear();
		const void* GetData() const;

		Uint64 GetDataSize() const;

		bool EndOfPacket() const;

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
		Int32 GetInt32();
		AString GetAString();
	private:
		bool CheckSize(Uint64 size);
		std::vector<Byte> data;
		Uint64 readPos, sendPos;
		bool isValid = true;
	};
	inline Int32 Packet::GetInt32()
	{
		if (this->data.size() < 5)
		{
			VarInt32 tmp{};
			for (int i = 0; i < this->data.size(); i++)
				tmp.bytes[i] = this->data[i];
			Int32 val = DecodeVarInt32(tmp, this->readPos);
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