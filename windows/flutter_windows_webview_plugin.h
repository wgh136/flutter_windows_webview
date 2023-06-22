#ifndef FLUTTER_PLUGIN_FLUTTER_WINDOWS_WEBVIEW_PLUGIN_H_
#define FLUTTER_PLUGIN_FLUTTER_WINDOWS_WEBVIEW_PLUGIN_H_

#include <windows.h>
#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>

#include <memory>

namespace flutter_windows_webview {

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

}  // namespace flutter_windows_webview

#endif  // FLUTTER_PLUGIN_FLUTTER_WINDOWS_WEBVIEW_PLUGIN_H_
