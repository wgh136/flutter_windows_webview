#include "eventChannel.h"

namespace Webview {
flutter_windows_webview::WebviewStreamHandler* handler = nullptr;

void sendMessage(const char* message) {
  if(handler != nullptr) {
    handler->sendMessage(message);
  }
}

void sendMessage(const wchar_t* message) {
  if(handler != nullptr) {
    handler->sendMessage(message);
  }
}

flutter_windows_webview::WebviewStreamHandler* addHandler() {
  if(handler == nullptr)
    handler = new flutter_windows_webview::WebviewStreamHandler;
  return handler;
}

void removeHandler() {
    handler->closeStream();
}
}