#define MINIAUDIO_IMPLEMENTATION
#include "proton/audio.hh"
#include "proton/logman.hh"
#include "proton/resourcemanager.hh"


namespace Proton
{
Audio::Audio(const std::string &audioPath)
{
    this->sound = nullptr;
#if defined(__ANDROID__)
    this->audioPath = audioPath;
#else
    this->audioPath = "assets/" + audioPath;
#endif
}

Audio::~Audio()
{
    if (sound != nullptr)
    {
        ma_sound_uninit(sound);
        delete sound;
        sound = nullptr;
        LogNew(Warn,"Audio ", audioPath, " destroyed");
    }
}

bool Audio::isPlaying() const
{
    if (sound == nullptr)
        return false;
    return ma_sound_is_playing(sound);
}

void Audio::setAudioPath(const char *newPath) { this->audioPath = newPath; }

void Audio::play()
{
    if (sound != nullptr)
    {
        ma_sound_stop(sound);
        ma_sound_uninit(sound);
        delete sound;
        sound = nullptr;
    }
    if (decoder != nullptr)
    {
        ma_decoder_uninit(decoder);
        delete decoder;
        decoder = nullptr;
    }

#if defined(__ANDROID__)
    AAssetManager *assetManager = ResourceManager::getInstance().getAssetManager();
    if (assetManager == nullptr)
    {
        Proton::Log("AAssetManager is null!! Unable to play sound on Android!!");
        return;
    }

    AAsset *asset = AAssetManager_open(assetManager, this->audioPath.c_str(), AASSET_MODE_BUFFER);
    if (asset == nullptr)
    {
        Proton::Log("Failed to open sound asset at ", this->audioPath);
        return;
    }

    size_t assetSize = AAsset_getLength(asset);
    void *audioData = malloc(assetSize);
    AAsset_read(asset, audioData, assetSize);
    AAsset_close(asset);

    this->decoder = new ma_decoder();
    ma_result result = ma_decoder_init_memory(audioData, assetSize, nullptr, this->decoder);

    if (result != MA_SUCCESS)
    {
        Proton::Log("Failed to init DECODER from memory for asset ", this->audioPath);
        free(audioData);
        delete this->decoder;
        this->decoder = nullptr;
        return;
    }

    this->sound = new ma_sound();
    result = ma_sound_init_from_data_source(ResourceManager::getInstance().getAudioEngine(), this->decoder, 0, nullptr,
                                            this->sound);

    if (result == MA_SUCCESS)
    {
        ma_sound_start(sound);
        Proton::Log("Sound started from memory ", this->audioPath);
    }
    else
    {
        Proton::Log("Failed to init SOUND from data source for asset ", this->audioPath);
        ma_decoder_uninit(this->decoder);
        delete this->decoder;
        this->decoder = nullptr;
        delete this->sound;
        this->sound = nullptr;
    }

    free(audioData);

#else

    this->decoder = new ma_decoder();
    ma_result result = ma_decoder_init_file(this->audioPath.c_str(), nullptr, this->decoder);

    if (result != MA_SUCCESS)
    {
        LogNew(Error, "Failed to init DECODER from file ", this->audioPath);
        delete this->decoder;
        this->decoder = nullptr;
        return;
    }

    this->sound = new ma_sound();
    result = ma_sound_init_from_data_source(ResourceManager::getInstance().getAudioEngine(), this->decoder, 0, nullptr,
                                            this->sound);

    if (result == MA_SUCCESS)
    {
        ma_sound_start(sound);
    }
    else
    {
        LogNew(Error, "Failed to init sound from data source for file ", this->audioPath);
        ma_decoder_uninit(this->decoder);
        delete this->decoder;
        this->decoder = nullptr;
        delete this->sound;
        this->sound = nullptr;
    }
#endif
}

void Audio::setVolume(const float volume) const {
    ma_sound_set_volume(this->sound, volume);
}

void Audio::setPitch(const float pitch) const {
    ma_sound_set_pitch(this->sound, pitch);
}

void Audio::stop() const
{
    if (sound != nullptr)
    {
        ma_sound_stop(sound);
    }

    if (decoder != nullptr)
    {
        ma_decoder_uninit(decoder);
        delete decoder;
    }
}
} // namespace Proton
