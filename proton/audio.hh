#pragma once
#include "logman.hh"
#include "miniaudio.h"
#include <thread>

namespace Proton
{
    extern ma_engine engine;

    void initAudioEngine();
    void destroyAudioEngine();

    class Audio
    {
    public:
        Audio(const char* audioPath);
        ~Audio();

        void setAudioPath(const char* newPath);
        void play();
        void stop();
        bool isPlaying() const;

    protected:
        static ma_sound* sound;
        const char* audioPath;
    };
}