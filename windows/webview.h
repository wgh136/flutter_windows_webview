#pragma once

#include <windows.h>
#include <wrl.h>
#include "WebView2.h"
#include "flutter_windows_webview_plugin.h"

using namespace Microsoft::WRL;

#define TitleFlag L"/r8A7g5E8dTitle"
#define CookieFlag L"/r8A7g5E8Cookie"
#define StatusFlag L"/r8A7g5E8Status"

namespace Webview {
    static const wchar_t* szWindowClass= L"Webview Test class";

    LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    HWND createWindow();

    void messageHandler(LPCWSTR message, int code);

    void createWebview(HWND hWnd, std::wstring initialUri, std::wstring proxy);

    void runScript(const char *script, int code);

    void navigateTo(const wchar_t *url);

    void closeWebview();

    bool isAvailable();

    void getCookies(const wchar_t* uri);

    void setCookie(const wchar_t *name, const wchar_t *value, const wchar_t *domain, const wchar_t *path);

    void close();

    void setUA(const char* ua);

    void setMethodChannel(std::unique_ptr<flutter::MethodChannel<>> channel);

    bool shouldBlockUri(std::wstring uri);

    bool shouldBlockNewWindow(std::wstring uri);
    
}