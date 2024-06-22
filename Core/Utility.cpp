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
	const char* base64Table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	void EncodeBase64(const char* input, Uint64 len, char* output)
	{
		int i, j;
		for (i = 0, j = 0; i < len;) {
			unsigned octet_a = i < len ? input[i++] : 0;
			unsigned octet_b = i < len ? input[i++] : 0;
			unsigned octet_c = i < len ? input[i++] : 0;

			unsigned triple = (octet_a << 16) + (octet_b << 8) + octet_c;

			output[j++] = base64Table[(triple >> 18) & 0x3F];
			output[j++] = base64Table[(triple >> 12) & 0x3F];
			output[j++] = base64Table[(triple >> 6) & 0x3F];
			output[j++] = base64Table[triple & 0x3F];
		}

		for (i = 0; i < (3 - len % 3) % 3; i++) {
			output[j - 1 - i] = '=';
		}
		output[j] = '\0';
	}
	Int32 base64_char_to_value(char c) {
		if ('A' <= c && c <= 'Z') return c - 'A';
		if ('a' <= c && c <= 'z') return c - 'a' + 26;
		if ('0' <= c && c <= '9') return c - '0' + 52;
		if (c == '+') return 62;
		if (c == '/') return 63;
		return -1; // Invalid base64 character
	}

	void DecodeBase64(const char* input, Byte* output, Int64* output_len)
	{
		int len = strlen(input);
		int i, j;
		int padding = 0;

		if (input[len - 1] == '=') padding++;
		if (input[len - 2] == '=') padding++;

		*output_len = (len / 4) * 3 - padding;

		for (i = 0, j = 0; i < len;) {
			unsigned sextet_a = base64_char_to_value(input[i++]);
			unsigned sextet_b = base64_char_to_value(input[i++]);
			unsigned sextet_c = base64_char_to_value(input[i++]);
			unsigned sextet_d = base64_char_to_value(input[i++]);

			if (sextet_a == -1 || sextet_b == -1 || (sextet_c == -1 && input[i - 2] != '=') || (sextet_d == -1 && input[i - 1] != '=')) {
				fprintf(stderr, "Invalid base64 input\n");
				exit(EXIT_FAILURE);
			}

			unsigned triple = (sextet_a << 18) + (sextet_b << 12) + (sextet_c << 6) + sextet_d;

			if (j < *output_len) output[j++] = (triple >> 16) & 0xFF;
			if (j < *output_len) output[j++] = (triple >> 8) & 0xFF;
			if (j < *output_len) output[j++] = triple & 0xFF;
		}
	}

	static int UnBase64(char c)
	{
		if ((c >= 'A') && (c <= 'Z'))
		{
			return c - 'A';
		}
		if ((c >= 'a') && (c <= 'z'))
		{
			return c - 'a' + 26;
		}
		if ((c >= '0') && (c <= '9'))
		{
			return c - '0' + 52;
		}
		if (c == '+')
		{
			return 62;
		}
		if (c == '/')
		{
			return 63;
		}
		if (c == '=')
		{
			return -1;
		}
		return -2;
	}
	std::string DecodeBase64(const std::string& a_Base64String)
	{
		std::string res;
		size_t i, len = a_Base64String.size();
		size_t o;
		int c;
		res.resize((len * 4) / 3 + 5, 0);  // Approximate the upper bound on the result length
		for (o = 0, i = 0; i < len; i++)
		{
			c = UnBase64(a_Base64String[i]);
			if (c >= 0)
			{
				switch (o & 7)
				{
				case 0: res[o >> 3] |= (c << 2); break;
				case 6: res[o >> 3] |= (c >> 4); res[(o >> 3) + 1] |= (c << 4); break;
				case 4: res[o >> 3] |= (c >> 2); res[(o >> 3) + 1] |= (c << 6); break;
				case 2: res[o >> 3] |= c; break;
				}
				o += 6;
			}
			if (c == -1)
			{
				// Error while decoding, invalid input. Return as much as we've decoded:
				res.resize(o >> 3);
				return res;
			}
		}
		res.resize(o >> 3);
		return res;
	}
	std::string EncodeBase64(const std::string& a_Input)
	{
		static const char BASE64[64] =
		{
			'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
			'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
			'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
			'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'
		};

		std::string output;
		output.resize(((a_Input.size() + 2) / 3) * 4);

		size_t output_index = 0;
		size_t size_full24 = (a_Input.size() / 3) * 3;

		for (size_t i = 0; i < size_full24; i += 3)
		{
			output[output_index++] = BASE64[static_cast<unsigned char>(a_Input[i]) >> 2];
			output[output_index++] = BASE64[(static_cast<unsigned char>(a_Input[i]) << 4 | static_cast<unsigned char>(a_Input[i + 1]) >> 4) & 63];
			output[output_index++] = BASE64[(static_cast<unsigned char>(a_Input[i + 1]) << 2 | static_cast<unsigned char>(a_Input[i + 2]) >> 6) & 63];
			output[output_index++] = BASE64[static_cast<unsigned char>(a_Input[i + 2]) & 63];
		}

		if (size_full24 < a_Input.size())
		{
			output[output_index++] = BASE64[static_cast<unsigned char>(a_Input[size_full24]) >> 2];
			if (size_full24 + 1 == a_Input.size())
			{
				output[output_index++] = BASE64[(static_cast<unsigned char>(a_Input[size_full24]) << 4) & 63];
				output[output_index++] = '=';
			}
			else
			{
				output[output_index++] = BASE64[(static_cast<unsigned char>(a_Input[size_full24]) << 4 | static_cast<unsigned char>(a_Input[size_full24 + 1]) >> 4) & 63];
				output[output_index++] = BASE64[(static_cast<unsigned char>(a_Input[size_full24 + 1]) << 2) & 63];
			}

			output[output_index++] = '=';
		}
		//ASSERT(output_index == output.size());

		return output;
	}
}