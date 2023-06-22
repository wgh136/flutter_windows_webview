#include "flutter_windows_webview_plugin.h"

// This must be included before many other Windows headers.
#include <windows.h>

// For getPlatformVersion; remove unless needed for your plugin implementation.
#include <VersionHelpers.h>

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

#include <memory>
#include <sstream>

#include "webview.h"

namespace flutter_windows_webview {

    HWND flutterWindow;

// static
void FlutterWindowsWebviewPlugin::RegisterWithRegistrar(
    flutter::PluginRegistrarWindows *registrar) {
    flutterWindow = registrar->GetView()->GetNativeWindow();
  auto channel =
      std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
          registrar->messenger(), "flutter_windows_webview",
          &flutter::StandardMethodCodec::GetInstance());

  auto plugin = std::make_unique<FlutterWindowsWebviewPlugin>();

  channel->SetMethodCallHandler(
      [plugin_pointer = plugin.get()](const auto &call, auto result) {
        plugin_pointer->HandleMethodCall(call, std::move(result));
      });

  registrar->AddPlugin(std::move(plugin));
}

FlutterWindowsWebviewPlugin::FlutterWindowsWebviewPlugin() {}

FlutterWindowsWebviewPlugin::~FlutterWindowsWebviewPlugin() {}

void FlutterWindowsWebviewPlugin::HandleMethodCall(
    const flutter::MethodCall<flutter::EncodableValue> &method_call,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    Webview().createWebview(flutterWindow, L"https://www.google.com");
}

}  // namespace flutter_windows_webview
