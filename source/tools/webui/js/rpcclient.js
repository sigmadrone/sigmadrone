RpcClient = function () {
  this.rpcFailureCount = 0;
  this.rpcCallInProggress = false;
}

RpcClient.prototype.rpcCall = function(url, method, params, successCallback, failCallback) {
  var request = {id: ++id, jsonrpc: "2.0"};
  request.method = method;
  request.params = params;
  if (request.params.length > 0) {
    request.params = JSON.parse(request.params);
  }
  /*console.log(JSON.stringify(request));*/
  this.rpcCallInProggress = true;
  this.successCallback = successCallback;
  var self = this;
  $.post(url, JSON.stringify(request), function(response) {
    self.rpcCallInProggress = false;
    self.rpcFailureCount = 0;
    if (successCallback) {
      successCallback(response);
    }
  }, "json").done(function(){
    //console.log("Success: " + method);
  }).fail(function(resp, error_type, error_text) {
    ++self.rpcFailureCount;
    self.rpcCallInProggress = false;
    console.log("Failed to execute " + method + "; err: " + error_type + "; details: " + error_text.message);
    if (failCallback != null) {
      failCallback(method);
    }
  });
}

RpcClient.prototype.isBusy = function() {
  return this.rpcCallInProggress;
}

RpcClient.prototype.failureCount = function() {
  return this.rpcFailureCount;
}
