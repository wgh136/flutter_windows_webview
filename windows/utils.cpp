#include "utils.h"
#include <Windows.h>

namespace flutter_windows_webview_utils {
    char* convertWcharToUTF8(const wchar_t* str)
    {
        int wideStrLen = static_cast<int>(wcslen(str) + 1);
        int mbStrLen = WideCharToMultiByte(CP_UTF8, 0, str, wideStrLen, NULL, 0, NULL, NULL);

        char* mbStr = new char[mbStrLen];
        WideCharToMultiByte(CP_UTF8, 0, str, wideStrLen, mbStr, mbStrLen, NULL, NULL);
        return mbStr;
    }

	std::string wstringToString(std::wstring str)
	{
        auto newCStr = convertWcharToUTF8(str.c_str());
        auto newStr = std::string{ newCStr };
        delete[] newCStr;
        return newStr;
	}

    wchar_t* convertUTF8ToWchar(const char* str) {
        int len = MultiByteToWideChar(CP_UTF8, 0, str, -1, nullptr, 0);
        auto wstr = new wchar_t[len];
        MultiByteToWideChar(CP_UTF8, 0, str, -1, wstr, len);
        return wstr;
    }

    std::wstring stringToWstring(std::string str) {
        auto newCWstr = convertUTF8ToWchar(str.c_str());
        auto newWstr = std::wstring{ newCWstr };
        delete[] newCWstr;
        return newWstr;
    }
}