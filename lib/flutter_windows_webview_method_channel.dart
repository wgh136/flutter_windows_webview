import 'package:flutter/foundation.dart';
import 'package:flutter/services.dart';
import 'flutter_windows_webview_platform_interface.dart';

/// An implementation of [FlutterWindowsWebviewPlatform] that uses method channels.
class MethodChannelFlutterWindowsWebview extends FlutterWindowsWebviewPlatform {
  /// The method channel used to interact with the native platform.
  @visibleForTesting
  final methodChannel = const MethodChannel('flutter_windows_webview');

  @override
  Future<String?> launchWebview() async {
    await methodChannel.invokeMethod<String>('start');
    return "ok";
  }
}
