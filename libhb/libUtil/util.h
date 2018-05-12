#pragma once

#include <string>

namespace common {
	namespace Util {
		std::wstring convertUtf8toUcs2(const std::string& str);
	}
}

#define UCS2(str) common::Util::convertUtf8toUcs2(str)