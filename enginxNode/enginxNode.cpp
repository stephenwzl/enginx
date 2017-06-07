#include <node.h>
extern "C" {
#include "enginx.h"
#include <stdlib.h>
}

namespace enginxNode {

using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;
using v8::Exception;
using v8::Value;

const char* ToCString(const String::Utf8Value& value) {
  return *value ? *value : "";
}

void LoadConfig(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  if (args.Length() != 1 || !args[0]->IsString()) {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "wrong type of argument")));
    return;
  }
  String::Utf8Value config(args[0]);
  enginx_load_config_string((char *)ToCString(config));
  enginx_compile_error* error = enginx_get_current_error();
  bool loadSuccess = error == NULL;
  if (!loadSuccess) {
    const char* error_msg = error->error_msg;
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, error_msg)));
    enginx_compile_error_release(&error);
    return;
  }
  args.GetReturnValue().Set(String::NewFromUtf8(isolate, error->error_msg));
}

void RewriteURL(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  if (args.Length() != 1 || !args[0]->IsString()) {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "wrong type of argument")));
    return;
  } 
  String::Utf8Value url(args[0]);
  char* rewrited_url = enginx_rewrite_url((char *)ToCString(url));
  args.GetReturnValue().Set(String::NewFromUtf8(isolate, rewrited_url));
  free(rewrited_url);
}

void init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "loadConfig", LoadConfig);
  NODE_SET_METHOD(exports, "rewriteURLString", RewriteURL);
}

NODE_MODULE(addon, init)

}
