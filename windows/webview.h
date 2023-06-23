#pragma once

#include <windows.h>
#include <wrl.h>
#include "WebView2.h"
#include "flutter_windows_webview_plugin.h"

using namespace Microsoft::WRL;

namespace Webview {
    static const wchar_t* szWindowClass= L"Webview Test class";

    LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    HWND createWindow();

    void messageHandler(LPCWSTR message, int code);

    void createWebview(HWND hWnd, const wchar_t *initialUri);

    void runScript(const char *script, int code);

    void navigateTo(const wchar_t *url);

    void closeWebview();

    bool isAvailable();
    
} // namespace Webview