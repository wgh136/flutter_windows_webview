import 'package:flutter/services.dart';

class Webview{
  static void startWebview([WebviewOptions? options]) async{
    listen(options?.messageReceiver, options?.onTitleChange);
    var channel = const MethodChannel("flutter_windows_webview");
    var res = await channel.invokeMethod("start");
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