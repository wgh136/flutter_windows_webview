import 'package:flutter/services.dart';
import 'package:flutter_windows_webview/src/webview_options.dart';

class FlutterWindowsWebview{
  static void Function(String)? cookieListener;
  bool isRunning = false;

  ///launch webview
  void launchWebview(String url, [WebviewOptions? options]) async{
    _listen(options?.messageReceiver, options?.onTitleChange);
    const MethodChannel("flutter_windows_webview").invokeMethod("start", url);
    isRunning = true;
  }

  ///listen message from c++
  void _listen(void Function(String)? messageReceiver, void Function(String)? onTitleChange) async{
    var channel = const EventChannel("flutter_windows_webview/message");
    await for(String message in channel.receiveBroadcastStream()){
      if(message.length > 15 && message.substring(0, 15)=="/r8A7g5E8dTitle"){
        if(onTitleChange!=null){
          onTitleChange.call(message.substring(15));
        }
        continue;
      }else if(message.length > 15 && message.substring(0, 15)=="/r8A7g5E8Cookie"){
        if(cookieListener != null){
          cookieListener!.call(message.substring(15));
        }
        continue;
      }else if(message.length > 15 && message.substring(0, 15)=="/r8A7g5E8Status"){
        if(message.substring(15) == " window close"){
          isRunning = false;
        }
        continue;
      }
      if(messageReceiver != null){
        messageReceiver.call(message);
      }
    }
  }

  ///confirm webview is available
  static Future<bool> isAvailable() async{
    var channel = const MethodChannel("flutter_windows_webview");
    var res = await channel.invokeMethod("isAvailable");
    return res;
  }

  ///run script
  void runScript(String script) async{
    if(!isRunning)  throw Exception("Webview is not running");
    var channel = const MethodChannel("flutter_windows_webview");
    await channel.invokeMethod("script", script);
  }

  ///get cookies
  Future<Map<String, String>> getCookies(String uri) async{
    if(!isRunning)  throw Exception("Webview is not running");
    if(cookieListener != null){
      throw Exception("Another cookie getting session is running");
    }
    String? cookies;
    cookieListener = (cookie) => cookies = cookie;
    var channel = const MethodChannel("flutter_windows_webview");
    await channel.invokeMethod("getCookies", uri);
    while(cookies == null){
      await Future.delayed(const Duration(milliseconds: 50));
    }
    cookieListener = null;
    try {
      if (cookies == "No cookies found.") {
        return {};
      } else {
        Map<String, String> result = {};
        var cookieList = cookies!.split(";");
        for (var cookie in cookieList) {
          var lr = cookie.split(":");
          result[lr[0]] = lr[1];
        }
        return result;
      }
    }
    catch(e){
      throw Exception("Cookies parsing failed");
    }
  }

  Future<void> close() async{
    if(!isRunning)  throw Exception("Webview is not running");
    var channel = const MethodChannel("flutter_windows_webview");
    var res = await channel.invokeMethod("close");
    isRunning = false;
  }

  Future<void> navigateTo(String uri) async{
    if(!isRunning)  throw Exception("Webview is not running");
    var channel = const MethodChannel("flutter_windows_webview");
    var res = await channel.invokeMethod("navigate", uri);
    isRunning = false;
  }
}