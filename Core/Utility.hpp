#pragma once
#include "Definitions.hpp"

namespace ugr
{
	union VarInt32
	{
		struct { Byte ub1, ub2, ub3, ub4, ub5; };
		struct { char b1, b2, b3, b4, b5; };
		Byte bytes[5];
	};
	VarInt32 EncodeVarInt32(Int32 value);
	Int32 DecodeVarInt32(VarInt32& vi, Uint64& index);
}