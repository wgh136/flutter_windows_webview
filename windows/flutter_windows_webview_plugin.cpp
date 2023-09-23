#include "flutter_windows_webview_plugin.h"
#include <windows.h>
#include <flutter/plugin_registrar_windows.h>
#include <memory>
#include "webview.h"
#include "eventChannel.h"

namespace flutter_windows_webview {

    // static
    void FlutterWindowsWebviewPlugin::RegisterWithRegistrar(
        flutter::PluginRegistrarWindows *registrar) {
        auto plugin = std::make_unique<FlutterWindowsWebviewPlugin>();
        auto channel = flutter::EventChannel<flutter::EncodableValue>(
            registrar->messenger(),
            "flutter_windows_webview/message",
            &flutter::StandardMethodCodec::GetInstance()
            );
        channel.SetStreamHandler(std::unique_ptr<WebviewStreamHandler>(Webview::addHandler()));
        
        auto channel2 =
            std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
            registrar->messenger(), "flutter_windows_webview",
            &flutter::StandardMethodCodec::GetInstance());
        

        channel2->SetMethodCallHandler(
            [plugin_pointer = plugin.get()](const auto &call, auto result) {
              plugin_pointer->HandleMethodCall(call, std::move(result));
        });

        Webview::setMethodChannel(std::move(channel2));
        

      registrar->AddPlugin(std::move(plugin));
    }

    FlutterWindowsWebviewPlugin::FlutterWindowsWebviewPlugin() = default;

    FlutterWindowsWebviewPlugin::~FlutterWindowsWebviewPlugin() = default;

    void FlutterWindowsWebviewPlugin::HandleMethodCall(
        const flutter::MethodCall<flutter::EncodableValue> &method_call,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
        auto method = method_call.method_name();
        if (method == "isAvailable") {
            result->Success(Webview::isAvailable());
        } else if (method == "start") {
            auto url = std::get_if<std::string>(method_call.arguments());
            auto str = url->c_str();
            int len = MultiByteToWideChar(CP_UTF8, 0, str, -1, nullptr, 0);
            auto wstr = new wchar_t[len];
            if (MultiByteToWideChar(CP_UTF8, 0, str, -1, wstr, len) == 0) {
                delete[] wstr;
                result->Success(flutter::EncodableValue("error"));
            }
            auto hwnd = Webview::createWindow();
            Webview::createWebview(hwnd, wstr);
            result->Success(flutter::EncodableValue("success"));
        } else if (method == "script") {
            auto script = std::get_if<std::string>(method_call.arguments());
            Webview::runScript(script->c_str(), 1234566);
            result->Success(flutter::EncodableValue("success"));
        } else if (method == "getCookies") {
            auto url = std::get_if<std::string>(method_call.arguments());
            auto str = url->c_str();
            int len = MultiByteToWideChar(CP_UTF8, 0, str, -1, nullptr, 0);
            auto wstr = new wchar_t[len];
            if (MultiByteToWideChar(CP_UTF8, 0, str, -1, wstr, len) == 0) {
                delete[] wstr;
                result->Success(flutter::EncodableValue("error"));
            }
            Webview::getCookies(wstr);
            result->Success(flutter::EncodableValue("success"));
        } else if (method == "navigate") {
            auto url = std::get_if<std::string>(method_call.arguments());
            auto str = url->c_str();
            int len = MultiByteToWideChar(CP_UTF8, 0, str, -1, nullptr, 0);
            auto wstr = new wchar_t[len];
            if (MultiByteToWideChar(CP_UTF8, 0, str, -1, wstr, len) == 0) {
                delete[] wstr;
                result->Success(flutter::EncodableValue("error"));
            }
            Webview::navigateTo(wstr);
            result->Success(flutter::EncodableValue("success"));
        } else if (method == "close") {
            Webview::close();
            result->Success(flutter::EncodableValue("success"));
        }
        else if (method == "setUA") {
            auto ua = std::get_if<std::string>(method_call.arguments());
            auto str = ua->c_str();
            Webview::setUA(str);
            result->Success(flutter::EncodableValue("success"));
        }
        else {
            result->Success(flutter::EncodableValue("success"));
        }
    }

}
