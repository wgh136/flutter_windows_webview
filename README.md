# flutter_windows_webview

A plugin that provides a webview interface for Flutter on Windows.

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

FlutterWindowsWebview.launchWebview("https://www.google.com");
```
Then the webview window will be created.

## Run script
Before running the script, make sure the webview is running.

#### use script
The following code shows how to run a script:
```
import 'package:flutter_windows_webview/flutter_windows_webview.dart';

FlutterWindowsWebview.runScript("alert(\"Hello World\")");
```

#### Sending message from the webview
You can send a message from the webview:
```
import 'package:flutter_windows_webview/flutter_windows_webview.dart';

FlutterWindowsWebview.runScript("window.chrome.webview.postMessage(\"Hello World\")");
```

If you want to receive these messages, you need to provide a messageReceiver when creating the webview:

```
import 'package:flutter_windows_webview/flutter_windows_webview.dart';

FlutterWindowsWebview.launchWebview("https://www.google.com", WebviewOptions(messageReceiver: (message) => print(message)));
```

## listen title's change
You need to provide an `onTitleChange` callback when creating the webview:
```
import 'package:flutter_windows_webview/flutter_windows_webview.dart';

FlutterWindowsWebview.launchWebview("https://www.google.com", WebviewOptions(onTitleChange: (message) => print(message)));
```

## cookies

### Get cookie
```
import 'package:flutter_windows_webview/flutter_windows_webview.dart';

var res = FlutterWindowsWebview.getCookies("https://www.google.com"));
print(res);
```