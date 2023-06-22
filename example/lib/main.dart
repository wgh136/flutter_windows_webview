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
  final _flutterWindowsWebviewPlugin = FlutterWindowsWebview();

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: const Text('Plugin example app'),
        ),
        body: Center(
          child: FilledButton(
            child: const Text("Test"),
            onPressed: (){
              _flutterWindowsWebviewPlugin.launchWebview();
            },
          ),
        ),
      ),
    );
  }
}
