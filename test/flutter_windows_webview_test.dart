import 'package:flutter_test/flutter_test.dart';
import 'package:flutter_windows_webview/flutter_windows_webview.dart';
import 'package:flutter_windows_webview/flutter_windows_webview_platform_interface.dart';
import 'package:flutter_windows_webview/flutter_windows_webview_method_channel.dart';
import 'package:plugin_platform_interface/plugin_platform_interface.dart';

class MockFlutterWindowsWebviewPlatform
    with MockPlatformInterfaceMixin
    implements FlutterWindowsWebviewPlatform {

  @override
  Future<String?> launchWebview() => Future.value('42');
}

void main() {
  final FlutterWindowsWebviewPlatform initialPlatform = FlutterWindowsWebviewPlatform.instance;

  test('$MethodChannelFlutterWindowsWebview is the default instance', () {
    expect(initialPlatform, isInstanceOf<MethodChannelFlutterWindowsWebview>());
  });

  test('getPlatformVersion', () async {
    FlutterWindowsWebview flutterWindowsWebviewPlugin = FlutterWindowsWebview();
    MockFlutterWindowsWebviewPlatform fakePlatform = MockFlutterWindowsWebviewPlatform();
    FlutterWindowsWebviewPlatform.instance = fakePlatform;

    expect('42', '42');
  });
}
