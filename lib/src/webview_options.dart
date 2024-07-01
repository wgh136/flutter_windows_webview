class WebviewOptions{
  ///initial width
  double? width;
  ///initial height;
  double? height;
  /// receive message from webview
  ///
  /// You can use the javascript "window.chrome.webview.postMessage()" to send message from webview;
  void Function(String)? messageReceiver;

  ///listen document's title change
  void Function(String)? onTitleChange;

  /// callback when webview try to navigate to another page,
  /// return true to prevent navigation.
  bool Function(String url)? onNavigation;

  /// set http proxy for webview as format "domain:port"
  String? proxy;

  /// Webview creating options
  WebviewOptions({this.width, this.height, this.messageReceiver,
    this.onNavigation, this.onTitleChange, this.proxy});
}