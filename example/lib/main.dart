import 'package:flutter/material.dart';
import 'package:flutter_windows_webview/flutter_windows_webview.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatefulWidget {
  const MyApp({super.key});

  @override
  State<MyApp> createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  var webview = FlutterWindowsWebview();

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: const Text('Plugin example app'),
        ),
        body: Column(
          children: [
            Center(
              child: FilledButton(
                child: const Text("1"),
                onPressed: () {
                  webview.launchWebview("https://google.com",
                      WebviewOptions(onNavigation: (url) {
                    debugPrint(url);
                    return false;
                  }));
                },
              ),
            ),
            Center(
              child: FilledButton(
                child: const Text("2"),
                onPressed: () async {
                  debugPrint(
                      '${await webview.getCookies("https://google.com")}');
                },
              ),
            ),
            Center(
              child: FilledButton(
                child: const Text("3"),
                onPressed: () async {
                  await webview.setCookie(
                    name: "test",
                    value: "hello",
                    domain: ".google.com",
                  );
                },
              ),
            ),
          ],
        ),
      ),
    );
  }
}
