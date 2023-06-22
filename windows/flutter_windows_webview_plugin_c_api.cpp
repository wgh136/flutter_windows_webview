#include "include/flutter_windows_webview/flutter_windows_webview_plugin_c_api.h"

#include <flutter/plugin_registrar_windows.h>

#include "flutter_windows_webview_plugin.h"

void FlutterWindowsWebviewPluginCApiRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  flutter_windows_webview::FlutterWindowsWebviewPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
