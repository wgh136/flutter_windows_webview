import 'package:flutter_windows_webview/src/webview.dart';

export 'package:flutter_windows_webview/src/webview.dart';

class FlutterWindowsWebview {
  static bool isRunning = false;
  ///launch Webview
  void launchWebview([WebviewOptions? options]) {
    isRunning = true;
    Webview.startWebview(options);
  }

  ///Check if Webview is available
  Future<bool> isAvailable(){
    return Webview.isAvailable();
  }

  void runScript(String script){
    if(!isRunning){
      throw Exception("Webview is not running");
    }
    Webview.runScript(script);
  }
}
