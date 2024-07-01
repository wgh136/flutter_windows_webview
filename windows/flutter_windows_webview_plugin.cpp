#include "flutter_windows_webview_plugin.h"
#include <windows.h>
#include <flutter/plugin_registrar_windows.h>
#include <memory>
#include "webview.h"
#include "eventChannel.h"
#include "utils.h"

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
            auto params = std::get<flutter::EncodableList>(*method_call.arguments());
            auto url = flutter_windows_webview_utils::stringToWstring(std::get<std::string>(params[0]));
            auto proxy = flutter_windows_webview_utils::stringToWstring(std::get<std::string>(params[1]));
            auto hwnd = Webview::createWindow();
            Webview::createWebview(hwnd, url, proxy);
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
        }else if(method == "setCookie"){
            auto args = std::get_if<flutter::EncodableMap>(method_call.arguments());
            auto name = std::get<std::string>(args->at(flutter::EncodableValue("name")));
            auto value = std::get<std::string>(args->at(flutter::EncodableValue("value")));
            auto domain = std::get<std::string>(args->at(flutter::EncodableValue("domain")));
            auto path = std::get<std::string>(args->at(flutter::EncodableValue("path")));
            auto wname = name.c_str();
            auto wvalue = value.c_str();
            auto wdomain = domain.c_str();
            auto wpath = path.c_str();
            int len = MultiByteToWideChar(CP_UTF8, 0, wname, -1, nullptr, 0);
            auto wstr = new wchar_t[len];
            if (MultiByteToWideChar(CP_UTF8, 0, wname, -1, wstr, len) == 0) {
                delete[] wstr;
                result->Success(flutter::EncodableValue("error"));
            }
            int len2 = MultiByteToWideChar(CP_UTF8, 0, wvalue, -1, nullptr, 0);
            auto wstr2 = new wchar_t[len2];
            if (MultiByteToWideChar(CP_UTF8, 0, wvalue, -1, wstr2, len2) == 0) {
                delete[] wstr2;
                result->Success(flutter::EncodableValue("error"));
            }
            int len3 = MultiByteToWideChar(CP_UTF8, 0, wdomain, -1, nullptr, 0);
            auto wstr3 = new wchar_t[len3];
            if (MultiByteToWideChar(CP_UTF8, 0, wdomain, -1, wstr3, len3) == 0) {
                delete[] wstr3;
                result->Success(flutter::EncodableValue("error"));
            }
            int len4 = MultiByteToWideChar(CP_UTF8, 0, wpath, -1, nullptr, 0);
            auto wstr4 = new wchar_t[len4];
            if (MultiByteToWideChar(CP_UTF8, 0, wpath, -1, wstr4, len4) == 0) {
                delete[] wstr4;
                result->Success(flutter::EncodableValue("error"));
            }
            Webview::setCookie(wstr, wstr2, wstr3, wstr4);
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
