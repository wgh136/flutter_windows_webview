import 'package:flutter_windows_webview/src/webview.dart';
import 'package:flutter_windows_webview/src/webview_options.dart';

export 'package:flutter_windows_webview/src/webview_options.dart';

class FlutterWindowsWebview {
  static bool _isRunning = false;
  ///launch Webview
  static void launchWebview(String url, [WebviewOptions? options]) {
    _isRunning = true;
    Webview.startWebview(url, options);
  }

  ///Check if Webview is available
  static Future<bool> isAvailable(){
    return Webview.isAvailable();
  }

  ///run script
  ///
  /// If you want to send message from webview, run script `window.chrome.webview.postMessage("Your message here")`
  ///
  /// To receive message, you need to provide param `messageReceiver` when create webview.
  static void runScript(String script){
    if(!_isRunning){
      throw Exception("Webview is not running");
    }
    Webview.runScript(script);
  }

  /// get cookies via specific uri
  static Future<Map<String, String>> getCookies(String uri){
    if(!_isRunning){
      throw Exception("Webview is not running");
    }
    return Webview.getCookies(uri);
  }

  ///check if webview is running
  static bool isRunning() => _isRunning;
}
