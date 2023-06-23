import 'package:flutter/services.dart';

class Webview{
  static void Function(String)? cookieListener;

  static void startWebview(String url, [WebviewOptions? options]) async{
    listen(options?.messageReceiver, options?.onTitleChange);
    var channel = const MethodChannel("flutter_windows_webview");
    var res = await channel.invokeMethod("start", url);
  }

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

  static Future<bool> isAvailable() async{
    var channel = const MethodChannel("flutter_windows_webview");
    var res = await channel.invokeMethod("isAvailable");
    return res;
  }

  static void runScript(String script) async{
    var channel = const MethodChannel("flutter_windows_webview");
    var res = await channel.invokeMethod("script", script);
  }

  static Future<String> getCookies(String uri) async{
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
    return cookies!;
  }
}

class WebviewOptions{
  ///initial width
  double? width;
  ///initial height;
  double? height;
  /// receive message from webview
  ///
  /// You can use the javascript "window.chrome.webview.postMessage()" to send message from webview;
  void Function(String)? messageReceiver;

  void Function(String)? onTitleChange;

  WebviewOptions({this.width, this.height, this.messageReceiver, this.onTitleChange});
}