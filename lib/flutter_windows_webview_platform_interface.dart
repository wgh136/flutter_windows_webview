import 'package:plugin_platform_interface/plugin_platform_interface.dart';

import 'flutter_windows_webview_method_channel.dart';

abstract class FlutterWindowsWebviewPlatform extends PlatformInterface {
  /// Constructs a FlutterWindowsWebviewPlatform.
  FlutterWindowsWebviewPlatform() : super(token: _token);

  static final Object _token = Object();

  static FlutterWindowsWebviewPlatform _instance = MethodChannelFlutterWindowsWebview();

  /// The default instance of [FlutterWindowsWebviewPlatform] to use.
  ///
  /// Defaults to [MethodChannelFlutterWindowsWebview].
  static FlutterWindowsWebviewPlatform get instance => _instance;

  /// Platform-specific implementations should set this with their own
  /// platform-specific class that extends [FlutterWindowsWebviewPlatform] when
  /// they register themselves.
  static set instance(FlutterWindowsWebviewPlatform instance) {
    PlatformInterface.verifyToken(instance, _token);
    _instance = instance;
  }

  Future<String?> launchWebview() {
    throw UnimplementedError('platformVersion() has not been implemented.');
  }
}
