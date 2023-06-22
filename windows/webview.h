#pragma once

#include <windows.h>
#include <string>
#include <wrl.h>
#include "WebView2.h"

using namespace Microsoft::WRL;

enum
{
    TitleChange,
        
};

class Webview {
    ICoreWebView2* webview = nullptr;
    ICoreWebView2Controller* webviewController = nullptr;

public:
    void messageHandler(LPCWSTR message, int code) {
      // TODO
    }

    void createWebview(HWND hWnd, const wchar_t *initialUri) {
      CreateCoreWebView2EnvironmentWithOptions(
          nullptr, nullptr, nullptr,
          Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
              [hWnd, initialUri, this](HRESULT result,
                                 ICoreWebView2Environment *env) -> HRESULT {
                // Create a CoreWebView2Controller and get the associated
                // CoreWebView2 whose parent is the main window hWnd
                env->CreateCoreWebView2Controller(
                    hWnd,
                    Callback<
                        ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                        [hWnd, initialUri, this](
                            HRESULT result,
                            ICoreWebView2Controller *controller) -> HRESULT {
                          if (controller != nullptr) {
                            webviewController = controller;
                            webviewController->get_CoreWebView2(&webview);
                          }

                          // Add a few settings for the webview
                          // The demo step is redundant since the values are the
                          // default settings
                          ICoreWebView2Settings* settings;
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

                          // <NavigationEvents>
                          // Step 4 - Navigation events
                          // register an ICoreWebView2NavigationStartingEventHandler
                          // to cancel any non-https navigation
                          EventRegistrationToken token;
                          webview->add_NavigationStarting(
                              Callback<ICoreWebView2NavigationStartingEventHandler>(
                                  [](ICoreWebView2 *webview,
                                     ICoreWebView2NavigationStartingEventArgs *args)
                                      -> HRESULT {
                                    LPWSTR uri;
                                    args->get_Uri(&uri);
                                    const std::wstring source(uri);
                                    if (source.substr(0, 5) != L"https") {
                                      args->put_Cancel(true);
                                    }
                                    return S_OK;
                                  })
                                  .Get(),
                              &token);
                          // </NavigationEvents>
                            
                          webview->add_WebMessageReceived(Callback<ICoreWebView2WebMessageReceivedEventHandler>(
                              [this](ICoreWebView2* webview, ICoreWebView2WebMessageReceivedEventArgs* args) -> HRESULT {
                                  LPWSTR message;
                                  args->TryGetWebMessageAsString(&message);
                                  // processMessage(&message);
                                  messageHandler(message, TitleChange);
                                  webview->PostWebMessageAsString(message);
                                  return S_OK;
                              }).Get(), &token);
                            
                          webview->AddScriptToExecuteOnDocumentCreated(
                              L"window.onload = function(){"
                              "window.chrome.webview.postMessage(window.document.title);"
                              "}",
                              nullptr);

                          return S_OK;
                        })
                        .Get());
                return S_OK;
              })
              .Get());
    }

    void runScript(const wchar_t *script, int code) {
      if (webview == nullptr) {
        throw std::exception("Webview is not running");
      }
      webview->ExecuteScript(
          script,
          Callback<ICoreWebView2ExecuteScriptCompletedHandler>(
              [code, this](HRESULT errorCode, LPCWSTR resultObjectAsJson) -> HRESULT {
                messageHandler(resultObjectAsJson, code);
                return S_OK;
              })
              .Get());
    }

    void navigateTo(const wchar_t *url) {
      if (webview == nullptr) {
        throw std::exception("Webview is not running");
      }
      webview->Navigate(url);
    }
};