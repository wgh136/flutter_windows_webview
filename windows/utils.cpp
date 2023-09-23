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
}