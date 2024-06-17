#include "Utility.hpp"

namespace ugr
{
	VarInt32 EncodeVarInt32(Int32 value)
	{
		VarInt32 vi{};
		Int32 count = 0;
		Uint32 i = value;
		do {
			Byte tmp = (i & 0b01111111);
			i >>= 7;
			if (i != 0) tmp |= 0b10000000;
			vi.bytes[count] = tmp;
			count++;
		} while (i != 0);
		return vi;
	}
	Int32 DecodeVarInt32(VarInt32& vi, Uint64& index)
	{
		Int32 result = 0;
		Byte b;
		do {
			if (index >= 5) break;
			b = vi.bytes[index];
			Int32 value = (b & 0b01111111);
			result |= (value << (7 * index));
			index++;
		} while ((b & 0b10000000) != 0);
		return result;
	}
}