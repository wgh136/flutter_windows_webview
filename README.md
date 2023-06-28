# flutter_windows_webview

**This package is not complete, do not use it in production.**

A plugin that provides a webview interface for Flutter on Windows.

Currently, this plugin only provides some basic functionality.

## Getting Started

### Check if webview is available
Webview is not supported on all versions of Windows. Use the following code to check if it is available.

```dart
import 'package:flutter_windows_webview/flutter_windows_webview.dart';

print(FlutterWindowsWebview.isAvailable());
```

### Create webview
Run the following code;
```dart
import 'package:flutter_windows_webview/flutter_windows_webview.dart';

FlutterWindowsWebview().launchWebview("https://www.google.com");
```
Then the webview window will be created.

## Run script
Before running the script, make sure the webview is running.

#### use script
The following code shows how to run a script:
```dart
import 'package:flutter_windows_webview/flutter_windows_webview.dart';

var webview = FlutterWindowsWebview();
webview.launchWebview("https://www.google.com");
webview.runScript("alert(\"Hello World\")");
```

#### Sending message from the webview
You can send a message from the webview:
```dart
import 'package:flutter_windows_webview/flutter_windows_webview.dart';

var webview = FlutterWindowsWebview();
webview.launchWebview("https://www.google.com");
webview.runScript("window.chrome.webview.postMessage(\"Hello World\")");
```

If you want to receive these messages, you need to provide a messageReceiver when creating the webview:

```dart
import 'package:flutter_windows_webview/flutter_windows_webview.dart';

FlutterWindowsWebview().launchWebview("https://www.google.com", WebviewOptions(messageReceiver: (message) => print(message)));
```

## listen title's change
You need to provide an `onTitleChange` callback when creating the webview:
```dart
import 'package:flutter_windows_webview/flutter_windows_webview.dart';

FlutterWindowsWebview().launchWebview("https://www.google.com", WebviewOptions(onTitleChange: (message) => print(message)));
```

## cookies

### Get cookie
```dart
import 'package:flutter_windows_webview/flutter_windows_webview.dart';

var webview = FlutterWindowsWebview();
webview.launchWebview("https://www.google.com");
var res = webview.getCookies("https://www.google.com");
print(res);
```

## Navigate

```dart
import 'package:flutter_windows_webview/flutter_windows_webview.dart';

var webview = FlutterWindowsWebview();
webview.launchWebview("https://www.google.com");
webview.navigateTo("https://www.bing.com");
```

## Close
```dart
import 'package:flutter_windows_webview/flutter_windows_webview.dart';

var webview = FlutterWindowsWebview();
webview.launchWebview("https://www.google.com");
webview.close();
```