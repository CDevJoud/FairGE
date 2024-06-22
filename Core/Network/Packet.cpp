#include "Packet.hpp"
#include <sstream>
#include "..\Debugger\Logger.hpp"

namespace ugr::Network
{
	using namespace Debugger;
	//Output
	/*Packet& Packet::operator>>(VarInt32& data)
	{
		if(!this->CheckSize(sizeof(data)))
		{
			int len = sizeof(data);
			std::memcpy(&data, &this->data[this->readPos], len);
		}
		return *this;
	}

	Packet& Packet::operator<<(VarInt32 data)
	{
		this->Append(&data, strlen(reinterpret_cast<const char*>(data.bytes)));
		return *this;
	}*/

	Packet& Packet::operator>>(Int32& data)
	{
		if (!this->CheckSize(data))
		{
			VarInt32 val;
			//TEMP!!!
			if(data <= 5)
			{
				switch (this->data.size()-this->readPos)
				{
				case 1:
					val = { this->data[this->readPos] };
					break;
				case 2:
					val = { this->data[this->readPos], this->data[this->readPos + 1] };
					break;
				case 3:
					val = { this->data[this->readPos], this->data[this->readPos + 1], this->data[this->readPos + 2] };
					break;
				case 4:
					val = { this->data[this->readPos], this->data[this->readPos + 1], this->data[this->readPos + 2], this->data[this->readPos + 3] };
					break;
				case 5:
					val = { this->data[this->readPos], this->data[this->readPos + 1], this->data[this->readPos + 2], this->data[this->readPos + 3], this->data[this->readPos + 4] };
					break;
				default:
					break;
				}
			}
			
			Uint64 index = 0;
			Int32 result = DecodeVarInt32(val, index);
			data = result;
			this->readPos += index;
		}
		return *this;
	}

	Packet& Packet::operator<<(Int32 data)
	{
		VarInt32 vi = EncodeVarInt32(data);
		this->Append(vi.bytes, strlen(reinterpret_cast<const char*>(vi.bytes)));
		return*this;
	}

	Packet& Packet::operator>>(AString& data)
	{
		Int32 length;
		*this >> length;
		return *this;
	}

	void Packet::Push(Int32 val)
	{
		VarInt32 vi = EncodeVarInt32(val);
		for (int i = 0; i < strlen(reinterpret_cast<const char*>(vi.bytes)); i++)
			this->data.push_back(vi.bytes[i]);
		if (val == 0)
			this->data.push_back(0);
	}

	void Packet::Push(AString str)
	{
		//Push the size of the string first!
		Push(str.size());
		for (int i = 0; i < str.size(); i++)
			this->data.push_back(str[i]);
	}

	/*void Packet::UnpackWithFormat(void* data, AString format)
	{
		Int32 packetsize = GetInt32();
		Byte* bData = reinterpret_cast<Byte*>(data);
		std::vector<AString> cmd;
		AString str;
		for (int i = 0; i < format.size() + 1; i++)
			if (format[i] != ' ')
				str.push_back(format[i]);
			else
			{
				cmd.push_back(str);
				str = "";
			}
		str.pop_back();
		cmd.push_back(str);
		Int32 bIndex = 0;

		for (auto& i : cmd)
			if (i == "Int32")
			{
				Int32 val = GetInt32();
				bData[bIndex] = reinterpret_cast<char*>(&val)[0]; bIndex++;
				bData[bIndex] = reinterpret_cast<char*>(&val)[1]; bIndex++;
				bData[bIndex] = reinterpret_cast<char*>(&val)[2]; bIndex++;
				bData[bIndex] = reinterpret_cast<char*>(&val)[3]; bIndex++;
			}
			else if (i == "AString")
			{
				AString Astrval = GetAString();
				for (int i = 0; i < sizeof(Astrval); i++)
					bData[bIndex + i] = reinterpret_cast<char*>(&Astrval)[i];
				bIndex += sizeof(Astrval);
			}
	}*/

	void Packet::UnpackWithFormat(void* data, AString format) {
		this->packsize = GetInt32();
		Byte* bData = reinterpret_cast<Byte*>(data);
		std::istringstream iss(format);
		std::vector<AString> cmd;
		AString token;

		while (iss >> token) {
			cmd.push_back(token);
		}

		Int32 bIndex = 0;

		for (const auto& i : cmd) {
			if (i == "Int32") {
				Int32 val = GetInt32();
				std::memcpy(&bData[bIndex], &val, sizeof(Int32));
				bIndex += sizeof(Int32);
			}
			else if (i == "AString") {
				AString Astrval = GetAString();
				std::memcpy(&bData[bIndex], &Astrval, sizeof(Astrval)); // +1 for null terminator
				bIndex += sizeof(Astrval);
			}
			else if (i == "RString")
			{
				AString tmpstr = this->GetAString();
				char* rstr = (char*)malloc(tmpstr.size() + 1);
				memset(rstr, 0, tmpstr.size() + 1);
				memcpy(rstr, tmpstr.c_str(), tmpstr.size() + 1);
				memcpy(&bData[bIndex], &rstr, sizeof(rstr));
				bIndex += sizeof(rstr);
			}
			else if (i == "Int16")
			{
				Int32 val = GetInt32(2);
				std::memcpy(&bData[bIndex], &val, sizeof(Int32));
				bIndex += sizeof(Int32);
			}
		}
		this->data.erase(this->data.begin(), this->data.begin() + this->packsize + 1);
		this->readPos = 0;
	}

	void Packet::Build()
	{
		VarInt32 size = EncodeVarInt32(this->data.size() + 1);
		switch (strlen(reinterpret_cast<char*>(size.bytes)))
		{
		case 1:
			this->data.insert(this->data.begin(), size.b1);
			break;
		case 2:
			this->data.insert(this->data.begin(), size.b2);
			this->data.insert(this->data.begin(), size.b1);
			break;
		case 3:
			this->data.insert(this->data.begin(), size.b3);
			this->data.insert(this->data.begin(), size.b2);
			this->data.insert(this->data.begin(), size.b1);
			break;
		case 4:
			this->data.insert(this->data.begin(), size.b4);
			this->data.insert(this->data.begin(), size.b3);
			this->data.insert(this->data.begin(), size.b2);
			this->data.insert(this->data.begin(), size.b1);
			break;
		case 5:
			this->data.insert(this->data.begin(), size.b5);
			this->data.insert(this->data.begin(), size.b4);
			this->data.insert(this->data.begin(), size.b3);
			this->data.insert(this->data.begin(), size.b2);
			this->data.insert(this->data.begin(), size.b1);
			break;
		}
	}

	bool Packet::CheckSize(Uint64 size)
	{
		this->isValid = this->isValid && (this->readPos + size <= this->data.size());
		return this->isValid;
	}

	Packet::Packet(Uint64 buffersize)
	{
		this->data.resize(buffersize);
	}

	Packet::Packet(AString str)
	{
		this->data.insert(this->data.end(), str.begin(), str.end());
	}

	void Packet::Append(const void* data, Uint64 size)
	{
		if (data && (size > 0))
		{
			const Byte* begin = reinterpret_cast<const Byte*>(data);
			const Byte* end = begin + size;
			this->data.insert(this->data.end(), begin, end);
		}
	}

	Uint64 Packet::GetReadPosition() const
	{
		return this->readPos;
	}

	void Packet::Clear()
	{
		this->data.clear();
		this->readPos = 0;
		this->isValid = true;
	}

	const void* Packet::GetBufferData() const
	{
		return !this->data.empty() ? this->data.data() : nullptr;
	}

	Uint64 Packet::GetBufferSize() const
	{
		return this->data.size();
	}

	Uint64 Packet::GetPacketSize() const
	{
		return this->packsize;
	}

	bool Packet::EndOfPacket() const
	{
		return this->readPos >= this->data.size();
	}

	void Packet::SetBufferDataSize(Uint64 size)
	{
		this->data.resize(size);
	}

	Packet::operator bool() const
	{
		return this->isValid;
	}
}