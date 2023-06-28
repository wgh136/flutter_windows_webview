#ifndef FLUTTER_PLUGIN_FLUTTER_WINDOWS_WEBVIEW_PLUGIN_H_
#define FLUTTER_PLUGIN_FLUTTER_WINDOWS_WEBVIEW_PLUGIN_H_

#include <codecvt>
#include <windows.h>
#include <flutter/method_channel.h>
#include <flutter/event_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <memory>
#include <flutter/standard_method_codec.h>
#include <string>

namespace flutter_windows_webview {

    inline char* convertWcharToUTF8(const wchar_t* str)
    {
        int wideStrLen = static_cast<int>(wcslen(str) + 1);
        int mbStrLen = WideCharToMultiByte(CP_UTF8, 0, str, wideStrLen, NULL, 0, NULL, NULL);

        char* mbStr = new char[mbStrLen];
        WideCharToMultiByte(CP_UTF8, 0, str, wideStrLen, mbStr, mbStrLen, NULL, NULL);
        return mbStr;
    }

    class FlutterWindowsWebviewPlugin : public flutter::Plugin {
     public:
      static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);

      FlutterWindowsWebviewPlugin();

      virtual ~FlutterWindowsWebviewPlugin();

      // Disallow copy and assign.
      FlutterWindowsWebviewPlugin(const FlutterWindowsWebviewPlugin&) = delete;
      FlutterWindowsWebviewPlugin& operator=(const FlutterWindowsWebviewPlugin&) = delete;

      // Called when a method is called on this plugin's channel from Dart.
      void HandleMethodCall(
          const flutter::MethodCall<flutter::EncodableValue> &method_call,
          std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    };

    class WebviewStreamHandler: public flutter::StreamHandler<>
    {
        std::unique_ptr<flutter::EventSink<>>&& streamEvents = nullptr;
        bool listening = false;
    protected:
        std::unique_ptr<flutter::StreamHandlerError<>> OnListenInternal(
            const flutter::EncodableValue* arguments, std::unique_ptr<flutter::EventSink<>>&& events) override
        {
            listening = true;
            streamEvents = std::move(events);
            return nullptr;
        }

        std::unique_ptr<flutter::StreamHandlerError<>> OnCancelInternal(const flutter::EncodableValue* arguments) override
        {
            listening = false;
            streamEvents = nullptr;
            return nullptr;
        }

    public:
        void sendMessage(const char* message) const
        {
            if(! listening) return;
            streamEvents->Success(flutter::EncodableValue(message));
        }

        void sendMessage(const wchar_t* message) const
        {
            if(! listening) return;
            const char* res = convertWcharToUTF8(message);
            streamEvents->Success(flutter::EncodableValue(res));
            delete res;
        }

        void closeStream()
        {
            if (this == nullptr) return;
            if (!listening) return;
            streamEvents->EndOfStream();
            listening = false;
        }
    };

}  // namespace flutter_windows_webview

#endif  // FLUTTER_PLUGIN_FLUTTER_WINDOWS_WEBVIEW_PLUGIN_H_
