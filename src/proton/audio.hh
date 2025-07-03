#pragma once

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
        std::string audioPath;
    };
}