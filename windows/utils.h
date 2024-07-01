#pragma once

#include <string>

namespace flutter_windows_webview_utils {
	std::string wstringToString(std::wstring str);
	std::wstring stringToWstring(std::string str);
}