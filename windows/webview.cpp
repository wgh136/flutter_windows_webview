#include "webview.h"
#include "WebView2.h"
#include "flutter_windows_webview_plugin.h"
#include "wil/com.h"
#include "wil/resource.h"
#include <windows.h>
#include <wrl.h>
#include <filesystem>
#include "eventChannel.h"
#include <flutter/method_channel.h>
#include "utils.h"
#include <future>

using namespace Microsoft::WRL;

namespace Webview {
wil::com_ptr<ICoreWebView2> webview = nullptr;
wil::com_ptr<ICoreWebView2Controller> webviewController = nullptr;
ICoreWebView2Settings* settings = nullptr;
HWND window = nullptr;
std::wstring user_agent{L""};
std::unique_ptr<flutter::MethodChannel<>> methodChannel = nullptr;

class WebviewMethodChannelResult : public flutter::MethodResult<flutter::EncodableValue> {
public:
    typedef std::function<void(const flutter::EncodableValue*)> SuccessCallback;

    SuccessCallback success_callback_;

    WebviewMethodChannelResult(SuccessCallback callback)  : success_callback_(std::move(callback)) {}

    ~WebviewMethodChannelResult() = default;

    void SuccessInternal(const flutter::EncodableValue* result) override {
        success_callback_(result);
    }

    void ErrorInternal(const std::string& error_code,
        const std::string& error_message,
        const flutter::EncodableValue* error_details) override {
        OutputDebugStringA((error_code + error_message).c_str());
        auto value = flutter::EncodableValue{ false };
        success_callback_(&value);
    }

    void NotImplementedInternal() override {
        OutputDebugString(L"no implement");
        auto value = flutter::EncodableValue{ false };
        success_callback_(&value);
    }
};

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
        
    switch (message)
    {
    case WM_DESTROY:
        DestroyWindow(hWnd);
        sendMessage("/r8A7g5E8Status window close");
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
    
    window_class.hIcon = LoadIcon(window_class.hInstance, MAKEINTRESOURCE(101));
    window_class.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    window_class.lpszMenuName = nullptr;
    window_class.hIconSm = LoadIcon(window_class.hInstance, MAKEINTRESOURCE(101));
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
    wchar_t appDataPath[MAX_PATH];
    GetEnvironmentVariableW(L"APPDATA", appDataPath, MAX_PATH);
    std::wstring path = std::wstring(appDataPath) + L"\\flutter_windows_webview";


    CreateCoreWebView2EnvironmentWithOptions(
        nullptr, path.c_str(), nullptr,
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
                        webview->get_Settings(&settings);
                        settings->put_IsScriptEnabled(TRUE);
                        settings->put_AreDefaultScriptDialogsEnabled(TRUE);
                        settings->put_IsWebMessageEnabled(TRUE);
                        if (user_agent.size() != 0) {
                            ICoreWebView2Settings2* settings2 = static_cast<ICoreWebView2Settings2*>(settings);
                            settings2->put_UserAgent(user_agent.c_str());
                        }

                        // Resize WebView to fit the bounds of the parent window
                        RECT bounds;
                        GetClientRect(hWnd, &bounds);
                        webviewController->put_Bounds(bounds);

                        webview->Navigate(initialUri);
                          delete[] initialUri;
                        EventRegistrationToken token;
                        webview->AddScriptToExecuteOnDocumentCreated(
                            L"window.onload = function(){"
                            "window.chrome.webview.postMessage(\"/r8A7g5E8dTitle\"+window.document."
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
                                      const wchar_t* flag = TitleFlag;
                                        bool changeTitle = true;
                                        for(int i=0;i<15;i++) {
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
                                            for(int i=15;i<length+1;i++) {
                                                title[i-15] = message[i];
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
                              }
                              EventRegistrationToken m_navigationStartingToken;

                              webview->add_NavigationStarting(
                                  Callback<ICoreWebView2NavigationStartingEventHandler>(
                                      [](ICoreWebView2* sender, ICoreWebView2NavigationStartingEventArgs* args)
                                      -> HRESULT {
                                          wil::unique_cotaskmem_string uri;
                                          args->get_Uri(&uri);

                                          if (shouldBlockUri(uri.get()))
                                          {
                                              args->put_Cancel(true);
                                          }
                                          return S_OK;
                                      }) .Get(), &m_navigationStartingToken);
                              return S_OK;
                          }
                          catch (...) {
                              return S_FALSE;
                          }
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

void getCookies(const wchar_t* uri) {
    ICoreWebView2* webview2;
    webviewController->get_CoreWebView2(&webview2);
    ICoreWebView2_2* web_view2_2 = static_cast<ICoreWebView2_2*>(webview2);
    ICoreWebView2CookieManager* m_cookieManager;
    web_view2_2->get_CookieManager(&m_cookieManager);
    m_cookieManager->GetCookies(
        uri,
        Callback<ICoreWebView2GetCookiesCompletedHandler>(
            [uri](HRESULT error_code, ICoreWebView2CookieList* list) -> HRESULT {

                std::wstring result = CookieFlag;
                UINT cookie_list_size;
                list->get_Count(&cookie_list_size);

                if (cookie_list_size == 0)
                {
                    result += L"No cookies found.";
                }
                else
                {
                    for (UINT i = 0; i < cookie_list_size; ++i)
                    {
                        wil::com_ptr<ICoreWebView2Cookie> cookie;
                        list->GetValueAtIndex(i, &cookie);

                        if (cookie.get())
                        {
                            LPWSTR name;
                            LPWSTR value;
                            cookie.get()->get_Name(&name);
                            cookie.get()->get_Value(&value);
                            result += name;
                            result += L":";
                            result += value;
                            if (i != cookie_list_size - 1)
                            {
                                result += L";";
                            }
                        }
                    }
                }
                const char* convertedValue = flutter_windows_webview::convertWcharToUTF8(result.c_str());
                sendMessage(convertedValue);
                return S_OK;
            })
            .Get());
}

void close() {
    if(window != nullptr)
        DestroyWindow(window);
}

void setUA(const char* ua) {
    std::string str{ua};
    std::wstring wstr(str.begin(), str.end());
    user_agent = wstr;
    if (settings != nullptr) {
        ICoreWebView2Settings2* settings2 = static_cast<ICoreWebView2Settings2*>(settings);
        settings2->put_UserAgent(wstr.c_str());
    }
}

void setMethodChannel(std::unique_ptr<flutter::MethodChannel<>> channel)
{
    methodChannel = std::move(channel);
}

bool waitingForChecking = false;

bool shouldBlockUri(std::wstring uri) {
    if (waitingForChecking) {
        waitingForChecking = false;
        return false;
    }
    auto value = std::make_unique<flutter::EncodableValue>(flutter::EncodableValue{ flutter_windows_webview_utils::wstringToString(uri) });

    waitingForChecking = true;
    methodChannel->InvokeMethod(std::string{ "navigation" }, std::move(value), std::make_unique<WebviewMethodChannelResult>([uri](const flutter::EncodableValue* result) {
        bool block = std::get<bool>(*result);
        if (!block) {
            webview->Navigate(uri.c_str());
        }
        else {
            waitingForChecking = false;
        }
     }));

    return true;
}
}
