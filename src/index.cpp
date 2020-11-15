#include "napi.h"
#include <string>
#include "yo.h"

Napi::String yoWhatsUp(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    std::string result = yoyoyo("NaM");

    return Napi::String::New(env, result);
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set(
        Napi::String::New(env, "yoWhatsUp"),
        Napi::Function::New(env, yoWhatsUp));

    return exports;
}

NODE_API_MODULE(index, Init)