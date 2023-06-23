﻿#include "webview.h"
#include "WebView2.h"
#include "flutter_windows_webview_plugin.h"
#include "wil/com.h"
#include "wil/resource.h"
#include <windows.h>
#include <wrl.h>

#include "eventChannel.h"

using namespace Microsoft::WRL;

namespace Webview {
wil::com_ptr<ICoreWebView2> webview = nullptr;
wil::com_ptr<ICoreWebView2Controller> webviewController = nullptr;
HWND window = nullptr;

enum
{
    TitleChange,
};

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
        
    switch (message)
    {
    case WM_DESTROY:
        DestroyWindow(hWnd);
        sendMessage("status: window close");
        removeHandler();
        break;
    case WM_SIZE:
        if (webviewController != nullptr) {
            RECT bounds;
            GetClientRect(hWnd, &bounds);
            webviewController->put_Bounds(bounds);
        }
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

HWND createWindow(){
    WNDCLASSEX window_class;
    auto hInstance = GetModuleHandle(nullptr);
    window_class.hCursor = LoadCursor(nullptr, IDC_ARROW);
    window_class.lpszClassName = szWindowClass;
    window_class.style = CS_HREDRAW | CS_VREDRAW;
    window_class.cbClsExtra = 0;
    window_class.cbWndExtra = 0;
    window_class.cbSize = sizeof(WNDCLASSEX);
    window_class.hInstance = hInstance;
    window_class.lpfnWndProc = WndProc;
    
    window_class.hIcon = LoadIcon(window_class.hInstance, IDI_APPLICATION);
    window_class.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    window_class.lpszMenuName = nullptr;
    window_class.hIconSm = LoadIcon(window_class.hInstance, IDI_APPLICATION);
    // The parameters to CreateWindow explained:
    // szWindowClass: the name of the application
    // szTitle: the text that appears in the title bar
    // WS_OVERLAPPEDWINDOW: the type of window to create
    // CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
    // 500, 100: initial size (width, length)
    // NULL: the parent of this window
    // NULL: this application does not have a menu bar
    // hInstance: the first parameter from WinMain
    // NULL: not used in this application
    RegisterClassEx(&window_class);
    HWND hWnd = CreateWindow(
        szWindowClass,
        L"Webview",
        WS_OVERLAPPEDWINDOW ,
        CW_USEDEFAULT, CW_USEDEFAULT,
        1200, 900,
        NULL,
        NULL,
        hInstance,
        NULL
    );
    window = hWnd;
    // The parameters to ShowWindow explained:
    // hWnd: the value returned from CreateWindow
    // nCmdShow: the fourth parameter from WinMain
    ShowWindow(hWnd,
        SW_SHOW);
    UpdateWindow(hWnd);
    return hWnd;
}

void messageHandler(LPCWSTR message, int code) {
    // TODO
}

void createWebview(HWND hWnd, const wchar_t *initialUri) {
    CreateCoreWebView2EnvironmentWithOptions(
        nullptr, L"D://webviewtest", nullptr,
        Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [hWnd, initialUri](HRESULT result,
                               ICoreWebView2Environment *env) -> HRESULT {
              // Create a CoreWebView2Controller and get the associated
              // CoreWebView2 whose parent is the main window hWnd
              env->CreateCoreWebView2Controller(
                  hWnd,
                  Callback<
                      ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                      [hWnd, initialUri](
                          HRESULT result,
                          ICoreWebView2Controller *controller) -> HRESULT {
                        if (controller != nullptr) {
                          webviewController = controller;
                          webviewController->get_CoreWebView2(&webview);
                        }

                        // Add a few settings for the webview
                        // The demo step is redundant since the values are the
                        // default settings
                        wil::com_ptr<ICoreWebView2Settings> settings;
                        webview->get_Settings(&settings);
                        settings->put_IsScriptEnabled(TRUE);
                        settings->put_AreDefaultScriptDialogsEnabled(TRUE);
                        settings->put_IsWebMessageEnabled(TRUE);

                        // Resize WebView to fit the bounds of the parent window
                        RECT bounds;
                        GetClientRect(hWnd, &bounds);
                        webviewController->put_Bounds(bounds);

                        // Schedule an async task to navigate to Bing
                        webview->Navigate(initialUri);
                          delete[] initialUri;
                        EventRegistrationToken token;
                        webview->AddScriptToExecuteOnDocumentCreated(
                            L"window.onload = function(){"
                            "window.chrome.webview.postMessage(\"title:\"+window.document."
                            "title);"
                            "}",
                            nullptr);
                          try {
                              const auto callback = Callback<
                                    ICoreWebView2WebMessageReceivedEventHandler>(
                                    [hWnd](ICoreWebView2* webview1,
                                       ICoreWebView2WebMessageReceivedEventArgs* args) -> HRESULT {
                                      LPWSTR message;
                                      args->TryGetWebMessageAsString(&message);
                                      sendMessage(message);
                                      const wchar_t* flag = L"title:";
                                        bool changeTitle = true;
                                        for(int i=0;i<6;i++) {
                                            if(message[i] == '\0') {
                                                changeTitle = false;
                                                break;
                                            }
                                            if(message[i] != flag[i]) {
                                                changeTitle = false;
                                                break;
                                            }
                                        }
                                        if(changeTitle) {
                                            int length = 0;
                                            while(message[length] != '\0') {
                                                length++;
                                            }
                                            wchar_t* title = new wchar_t[length-5];
                                            for(int i=6;i<length+1;i++) {
                                                title[i-6] = message[i];
                                            }
                                            SetWindowText(hWnd, title);
                                            delete[] title;
                                        }
                                      webview1->PostWebMessageAsString(message);
                                      return S_OK;
                                    });
                              if(callback != nullptr) {
                                   webview->add_WebMessageReceived(
                                      callback.Get(),
                                      &token);
                                  return S_OK;
                              }
                          }
                          catch (...) {
                              return S_FALSE;
                          }
                          return S_FALSE;
                      })
                      .Get());
              return S_OK;
            })
            .Get());
}

void runScript(const char *script, int code) {
    if (webview == nullptr) {
        throw std::exception("Webview is not running");
    }
    int len = MultiByteToWideChar(CP_UTF8, 0, script, -1, nullptr, 0);
    
    wchar_t* wstr = new wchar_t[len];
    
    if (MultiByteToWideChar(CP_UTF8, 0, script, -1, wstr, len) == 0) {
        std::cerr << "Failed to convert string from multibyte to wide character.\n";
        delete[] wstr;
    }
    webview->ExecuteScript(
        wstr,
        Callback<ICoreWebView2ExecuteScriptCompletedHandler>(
            [code](HRESULT errorCode, LPCWSTR resultObjectAsJson) -> HRESULT {
              messageHandler(resultObjectAsJson, code);
              return S_OK;
            })
            .Get());
    delete[] wstr;
}

void navigateTo(const wchar_t *url) {
    if (webview == nullptr) {
        throw std::exception("Webview is not running");
    }
    webview->Navigate(url);
}

void closeWebview() {
    if(window == nullptr)   return;
    DestroyWindow(window);
}

bool isAvailable() {
    wchar_t* res;
    GetAvailableCoreWebView2BrowserVersionString(nullptr, &res);
    if(res == nullptr) {
        return false;
    } else {
        return true;
    }
}


}