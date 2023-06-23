# flutter_windows_webview

A plugin that provides a webview interface on the flutter Windows.

## Getting Started

### Check if webview is available
Webview is not support all versions of Windows. Use following code to check if it is available.

```dart
import 'package:flutter_windows_webview/flutter_windows_webview.dart';

print(FlutterWindowsWebview().isAvailable());
```

### Create webview
Run following code;
```dart
import 'package:flutter_windows_webview/flutter_windows_webview.dart';

FlutterWindowsWebview().launchWebview();
```
Then the webview window will be created.

### Run script
Before run script, make sure the webview is running.

#### use script
The following code show how to run script;
```dart
import 'package:flutter_windows_webview/flutter_windows_webview.dart';

FlutterWindowsWebview().runScript("alert(\"Hello World\")");
```

#### send message from webview
You can send message from webview.
```dart
import 'package:flutter_windows_webview/flutter_windows_webview.dart';

FlutterWindowsWebview().runScript("window.chrome.webview.postMessage(\"Hello World\")");
```

If you want to receive these message, you need to provide `messageReceiver` when create webview;

```dart
import 'package:flutter_windows_webview/flutter_windows_webview.dart';

FlutterWindowsWebview().launchWebview(messageReceiver(messageReceiver: (message) => print(message)));;
```

### listen title's change
You need to provide `onTitleChange` when create webview;
```dart
import 'package:flutter_windows_webview/flutter_windows_webview.dart';

FlutterWindowsWebview().launchWebview(messageReceiver(onTitleChange: (message) => print(message)));;
```

### cookies
TODO