#pragma once

#include "logman.hh"
#include "miniaudio.h"

namespace Proton
{
    class Audio
    {
    public:
        Audio(std::string audioPath);
        ~Audio();

        void setAudioPath(const char *newPath);
        void play();
        void stop();
        bool isPlaying() const;

    protected:
        ma_sound *sound;
        std::string audioPath;
    };
}