#pragma once
#include <string>
#include <codecvt>
#pragma warning(disable : 4996)

namespace ugr
{
	using Int8 = char;
	using Int16 = short;
	using Int32 = int;
	using Int64 = long long;

	using Uint8 = unsigned char;
	using Uint16 = unsigned short;
	using Uint32 = unsigned int;
	using Uint64 = unsigned long long;

	using Byte = Uint8;
	class String : public std::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t>> {
	public:
		String() = default;

		String(const char* str) {
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> converter;
			this->append(converter.from_bytes(str));
		}

		~String() = default;

		String(const wchar_t* str) {
			this->append(str);
		}

		String(wchar_t* str) {
			this->append(str);
		}

		String& operator=(const std::wstring& str) {
			if (this == reinterpret_cast<const String*>(&str)) {
				return *this;
			}

			this->assign(str);
			return *this;
		}

		bool operator==(const char* str) const {
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> converter;
			return this->compare(converter.from_bytes(str)) == 0;
		}

		String operator+(const String& str) const {
			String result(*this);
			result.append(str);
			return result;
		}

		String operator+(const wchar_t* str) const {
			String result(*this);
			result.append(str);
			return result;
		}

		String operator+(const char* str) const {
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> converter;
			String result(*this);
			result.append(converter.from_bytes(str).c_str());
			return result;
		}

		friend String operator+(const char* lhs, const String& rhs) {
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> converter;
			String result(converter.from_bytes(lhs).c_str());
			result.append(rhs);
			return result;
		}
	};
	using AString = std::string;
	enum SocketStatus
	{
		Error, NotReady, Disconnected, Done, Partial
	};
}