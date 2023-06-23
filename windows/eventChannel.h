#pragma once

#include "flutter_windows_webview_plugin.h"

namespace Webview {
void sendMessage(const char* message);

void sendMessage(const wchar_t* message);

flutter_windows_webview::WebviewStreamHandler* addHandler();

void removeHandler();
}