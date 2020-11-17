#include <napi.h>
#include <string>
#include <stdint.h>
#include "../origin/c-interop/exports.h"

Napi::Boolean InitializeLibrary(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    auto clr1 = "/usr/bin/node";
    auto clr2 = "/usr/share/dotnet/shared/Microsoft.NETCore.App/3.1.10";
    auto clr3 = "/home/5E7EN/GitHub/Message-Height-Twitch-JS/origin/bin/Release/netstandard2.1/publish/MessageHeightTwitch.dll";

    auto res = LoadCLRRuntime(clr1, clr2, clr3);

    if (res != 0)
    {
        return Napi::Boolean::New(env, false);
    }

    auto charMapPath = "./origin/charmap.bin.gz";

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

    auto res = InitChannel((channelName).Utf8Value().c_str(), (channelID).Utf8Value().c_str(), timeout);

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
    if (info.Length() < 6)
    {
        Napi::TypeError::New(env, "Wrong number of arguments provided. Expected: channelName <string>, messageInput <string>, username <string>, displayName <string>, badgeCount <number>, twitchEmotes <array>, twitchEmotesLength <number>").ThrowAsJavaScriptException();
    }

    // Ensure provided arguments are valid
    if (!info[0].IsString() || !info[1].IsString() || !info[2].IsString() || !info[3].IsString() || !info[4].IsNumber() || !info[5].IsArray())
    {
        Napi::TypeError::New(env, "Some argument has invalid type. Expected: channelName <string>, messageInput <string>, username <string>, displayName <string>, badgeCount <number>, twitchEmotes <array>, twitchEmotesLength <number>").ThrowAsJavaScriptException();
    }

    auto channelName = info[0].As<Napi::String>();
    auto messageInput = info[1].As<Napi::String>();
    auto username = info[2].As<Napi::String>();
    auto displayName = info[3].As<Napi::String>();
    auto badgeCount = info[4].As<Napi::Number>().Int32Value();
    auto twitchEmotes = info[5].As<Napi::Array>();
    auto twitchEmotesLength = twitchEmotes.Length();

    TwitchEmote verifiedEmotes[twitchEmotesLength];
    std::string nameStringInstances[twitchEmotesLength];
    std::string urlStringInstances[twitchEmotesLength];

    for (auto i = 0; i < twitchEmotesLength; i++)
    {
        Napi::Value emote = twitchEmotes[i];

        if (!emote.IsObject())
        {
            Napi::TypeError::New(env, "Emotes list has item of invalid type. Expected object with 'Name' and 'Url' properties.").ThrowAsJavaScriptException();
        }

        Napi::Value targetName = emote.As<Napi::Object>()["name"];
        Napi::Value targetUrl = emote.As<Napi::Object>()["url"];

        if (targetName != nullptr && targetName.IsString())
        {
            auto utf8 = targetName.As<Napi::String>().Utf8Value();
            nameStringInstances[i] = utf8;
            verifiedEmotes[i].Name = nameStringInstances[i].c_str();
        }
        else
        {
            Napi::TypeError::New(env, "Emotes list has item of invalid type. Expected object with 'name' <string> and 'url' <string> properties.").ThrowAsJavaScriptException();
        }

        if (targetUrl != nullptr && targetUrl.IsString())
        {
            auto utf8 = targetUrl.As<Napi::String>().Utf8Value();
            urlStringInstances[i] = utf8;
            verifiedEmotes[i].Url = urlStringInstances[i].c_str();
        }
    }

    float res = CalculateMessageHeightDirect((channelName).Utf8Value().c_str(), (messageInput).Utf8Value().c_str(), (username).Utf8Value().c_str(), (displayName).Utf8Value().c_str(), badgeCount, verifiedEmotes, twitchEmotesLength);

    auto converted = Napi::Number::New(env, res);

    return Napi::Number::New(env, converted);
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set(
        Napi::String::New(env, "initializeLibrary"),
        Napi::Function::New(env, InitializeLibrary));

    exports.Set(
        Napi::String::New(env, "initializeChannel"),
        Napi::Function::New(env, InitializeChannel));

    exports.Set(
        Napi::String::New(env, "calculateHeight"),
        Napi::Function::New(env, CalculateHeight));

    return exports;
}

NODE_API_MODULE(index, Init)