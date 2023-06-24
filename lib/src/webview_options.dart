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

  /// Webview creating options
  WebviewOptions({this.width, this.height, this.messageReceiver, this.onTitleChange});
}