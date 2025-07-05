#pragma once

#if defined(__ANDROID__)
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#endif
#include "logman.hh"
#include "miniaudio.h"

namespace Proton
{
    class Audio
    {
    public:
        explicit Audio(const std::string &audioPath);
        ~Audio();

        void setAudioPath(const char *newPath);
        void play();
        void stop() const;
        [[nodiscard]] bool isPlaying() const;

    protected:
        ma_sound *sound;
        ma_decoder *decoder = nullptr;
        std::string audioPath;
    };
}