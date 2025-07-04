#pragma once
#include <iostream>
#include <sstream>

#if defined(__ANDROID__)
#include <android/log.h>
#define LOG_TAG "Proton2D"
#endif

namespace Proton
{
    template <typename... logArgs>
    void Log(logArgs &&...args)
    {
        std::stringstream ss;
        (ss << ... << args);

#if defined(__ANDROID__)
        __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "%s", ss.str().c_str());
#else
        std::cout << "[PROTON2D] " << ss.str() << std::endl;
#endif
    }
}