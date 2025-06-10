#define MINIAUDIO_IMPLEMENTATION
#include "audio.hh"

namespace Proton
{
    ma_engine engine;
    void initAudioEngine()
    {
        if (ma_engine_init(NULL, &engine) != MA_SUCCESS)
        {
            Proton::Log("Unable to init sound engine");
            return;
        }
        Proton::Log("miniaudio backend initialized successfully");
    }
    void destroyAudioEngine()
    {
        ma_engine_uninit(&engine);
        Proton::Log("Audio module shutted down");
    }

    ma_sound* Audio::sound = nullptr;

    Audio::Audio(const char* audioPath)
    {
        this->audioPath = audioPath;
    }

    Audio::~Audio()
    {
        if (sound != nullptr)
        {
            ma_sound_uninit(sound);
            delete sound;
            sound = nullptr;
            Proton::Log("Audio ", audioPath, " destroyed");
        }
    }

    bool Audio::isPlaying() const
    {
        if(sound == nullptr) return false;
        return ma_sound_is_playing(sound);
    }

    void Audio::setAudioPath(const char* newPath)
    {
        this->audioPath = newPath;
    }

    void Audio::play()
    {
        const char* audioPth = this->audioPath;

        if (sound != nullptr)
        {
            ma_sound_stop(sound);
            ma_sound_uninit(sound);
            delete sound;
            sound = nullptr;
        }

        std::thread([audioPth]() {
            sound = new ma_sound;
            if (ma_sound_init_from_file(&engine, audioPth, 0, NULL, NULL, sound) == MA_SUCCESS) {
                ma_sound_start(sound);
            } else {
                Proton::Log("Failed to load sound file ", audioPth);
                delete sound;
                sound = nullptr;
            }
        }).detach();
    }

    void Audio::stop()
    {
        if (sound != nullptr)
        {
            ma_sound_stop(sound);
        }
    }
}
