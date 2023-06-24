import 'package:flutter/services.dart';
import 'package:flutter_windows_webview/src/webview_options.dart';

class Webview{
  static void Function(String)? cookieListener;

  ///launch webview
  static void startWebview(String url, [WebviewOptions? options]) async{
    listen(options?.messageReceiver, options?.onTitleChange);
    var channel = const MethodChannel("flutter_windows_webview");
    var res = await channel.invokeMethod("start", url);
  }

  ///listen message from c++
  static void listen(void Function(String)? messageReceiver, void Function(String)? onTitleChange) async{
    var channel = const EventChannel("flutter_windows_webview/message");
    await for(String message in channel.receiveBroadcastStream()){
      if(message.length > 6 && message.substring(0, 6)=="title:"){
        if(onTitleChange!=null){
          onTitleChange.call(message.substring(6));
        }
        continue;
      }else if(message.length > 7 && message.substring(0, 7)=="status:"){
        continue;
      }else if(message.length > 7 && message.substring(0, 7)=="cookie:"){
        if(cookieListener != null){
          cookieListener!.call(message.substring(7));
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
  static void runScript(String script) async{
    var channel = const MethodChannel("flutter_windows_webview");
    var res = await channel.invokeMethod("script", script);
  }

  ///get cookies
  static Future<Map<String, String>> getCookies(String uri) async{
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
}