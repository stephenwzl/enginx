#include <node.h>
#include "../enginx/core/enginx.h"


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
  enginx::EnginxError error;
  bool loadSuccess = enginx::Enginx::load(ToCString(config), error);
  if (!loadSuccess) {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, error.message.c_str())));
    return;
  }
  args.GetReturnValue().Set(String::NewFromUtf8(isolate, error.message.c_str()));
}

void init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "loadConfig", LoadConfig);
}

NODE_MODULE(addon, init)

}
