#include <napi.h>
#include <string>
#include <stdint.h>
#include "../origin/c-interop/exports.h"

Napi::Boolean InitializeLibrary(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    auto clr1 = std::string("/usr/bin/node").c_str();
    auto clr2 = std::string("/usr/share/dotnet/shared/Microsoft.NETCore.App/3.1.10").c_str();
    auto clr3 = std::string("/home/5E7EN/GitHub/Message-Height-Twitch-JS/origin/bin/Release/netstandard2.1/publish/MessageHeightTwitch.dll").c_str();

    auto res = LoadCLRRuntime(clr1, clr2, clr3);

    if (res != 0)
    {
        return Napi::Boolean::New(env, false);
    }

    auto charMapPath = std::string("../origin/charmap.bin.gz").c_str();

    res = InitCharMap(charMapPath);

    if (res != 1)
    {
        return Napi::Boolean::New(env, false);
    }

    return Napi::Boolean::New(env, true);
}

Napi::Boolean InitializeChannel(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    // Ensure correct amount of args were provided
    if (info.Length() < 3)
    {
        Napi::TypeError::New(env, "Wrong number of arguments provided. Expected: channelName <string>, channelID <string>, timeout <number>").ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, false);
    }

    // Ensure provided arguments are valid
    if (!info[0].IsString() || !info[1].IsString() || !info[2].IsNumber())
    {
        Napi::TypeError::New(env, "Some argument has invalid type. Expected: channelName <string>, channelID <string>, timeout <number>").ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, false);
    }

    auto channelName = info[0].As<Napi::String>();
    auto channelID = info[1].As<Napi::String>();
    auto timeout = info[2].As<Napi::Number>().Int32Value();

    auto res = InitChannel(std::string(channelName).c_str(), std::string(channelID).c_str(), timeout);

    if (res != 1)
    {
        Napi::Error::New(env, "Something bad happened during initialization.").ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, false);
    }

    return Napi::Boolean::New(env, true);
}

Napi::Number CalculateHeight(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    // Ensure correct amount of args were provided
    if (info.Length() < 7)
    {
        Napi::TypeError::New(env, "Wrong number of arguments provided. Expected: channelName <string>, messageInput <string>, username <string>, displayName <string>, badgeCount <number>, twitchEmotes <array>, twitchEmotesLength <number>").ThrowAsJavaScriptException();
    }

    // Ensure provided arguments are valid
    if (!info[0].IsString() || !info[1].IsString() || !info[2].IsString() || !info[3].IsString() || !info[4].IsNumber() || !info[5].IsObject() || !info[6].IsNumber())
    {
        Napi::TypeError::New(env, "Some argument has invalid type. Expected: channelName <string>, messageInput <string>, username <string>, displayName <string>, badgeCount <number>, twitchEmotes <array>, twitchEmotesLength <number>").ThrowAsJavaScriptException();
    }

    auto channelName = info[0].As<Napi::String>();
    auto messageInput = info[1].As<Napi::String>();
    auto username = info[2].As<Napi::String>();
    auto displayName = info[3].As<Napi::String>();
    auto badgeCount = info[4].As<Napi::Number>().Int32Value();
    // TODO: FIX THIS
    auto TwitchEmotes = info[5];
    auto TwitchEmotesLen = info[6].As<Napi::Number>().Int32Value();

    //float res = CalculateMessageHeightDirect(std::string(channelName).c_str(), std::string(messageInput).c_str(), std::string(username).c_str(), std::string(displayName).c_str(), badgeCount, TwitchEmotes, TwitchEmotesLen);

    //auto converted = Napi::Number::New(env, res);

    return Napi::Number::New(env, 1);
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set(
        Napi::String::New(env, "InitializeLibrary"),
        Napi::Function::New(env, InitializeLibrary));

    exports.Set(
        Napi::String::New(env, "InitializeChannel"),
        Napi::Function::New(env, InitializeChannel));

    // exports.Set(
    //     Napi::String::New(env, "CalculateHeight"),
    //     Napi::Function::New(env, CalculateHeight));

    return exports;
}

NODE_API_MODULE(index, Init)