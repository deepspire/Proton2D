#define MINIAUDIO_IMPLEMENTATION
#include "audio.hh"
#include "resourcemanager.hh"

namespace Proton
{
    Audio::Audio(std::string audioPath)
    {
        this->sound = nullptr;
        this->audioPath = "assets/" + audioPath;
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
        if (sound == nullptr)
            return false;
        return ma_sound_is_playing(sound);
    }

    void Audio::setAudioPath(const char *newPath)
    {
        this->audioPath = newPath;
    }

    void Audio::play()
    {
        const char *audioPth = this->audioPath.c_str();

        if (sound != nullptr)
        {
            ma_sound_stop(sound);
            ma_sound_uninit(sound);
            delete sound;
            sound = nullptr;
        }

        this->sound = new ma_sound();
        if (ma_sound_init_from_file(ResourceManager::getInstance().getAudioEngine(), audioPth, 0, NULL, NULL, sound) == MA_SUCCESS)
        {
            ma_sound_start(sound);
        }
        else
        {
            Proton::Log("Failed to load sound file ", audioPth);
            delete sound;
            sound = nullptr;
        }
    }

    void Audio::stop()
    {
        if (sound != nullptr)
        {
            ma_sound_stop(sound);
        }
    }
}
