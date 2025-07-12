#pragma once

#include <cstdint>
#include <sstream>
#include <utility>
#include <iostream>
#include <vector>

#if defined(__ANDROID__)
#include <android/log.h>
#define LOG_TAG "Proton2D"
#endif

namespace Proton
{
enum LogInfo : uint8_t
{
    Info,
    Warn,
    Error,
};

struct Logged
{
    Logged(std::string msg, LogInfo level)
    {
        message = std::move(msg);
        logLevel = level;
    }

    LogInfo logLevel;
    std::string message;
};

extern std::vector<Logged> _protonLoggedVector;

template <typename... logArgs> void Log(logArgs &&...args)
{
    std::stringstream ss;
    (ss << ... << args);

    #if defined(__ANDROID__)
        __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "%s", ss.str().c_str());
    #else
        std::cout << "[PROTON2D] " << ss.str() << '\n';
    #endif
}

template <typename... logArgs> void LogNew(LogInfo level = LogInfo::Info, logArgs &&...args)
{
    std::stringstream ss;
    (ss << ... << args);

    Logged message(ss.str(), level);
    _protonLoggedVector.push_back(message);
}
} // namespace Proton