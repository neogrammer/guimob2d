//
// Created by jlhar on 3/16/2026.
//

#ifndef RAYMOB1_SOUNDMANAGER_H
#define RAYMOB1_SOUNDMANAGER_H


#include "AudioTypes.h"
#include "SoundRepository.h"
#include "SoundCache.h"

#include <string>



namespace sfx
{
    class SoundManager
    {
    public:
        bool init();
        void shutdown();
        void update();

        SoundRepository& repository();
        const SoundRepository& repository() const;

        bool loadAsset(const std::string& id);
        bool loadTag(const std::string& tag);

        void unloadAsset(const std::string& id);
        void unloadAll();

        void playSfx(const std::string& id, float volumeMultiplier = 1.0f);
        void playMusic(const std::string& id, bool restartIfPlaying = false);
        void stopMusic(const std::string& id);
        void stopAllMusic();

        void setMasterVolume(float v);
        void setBusVolume(AudioBus bus, float v);

        float masterVolume() const;
        float busVolume(AudioBus bus) const;

    private:
        float effectiveVolume(AudioBus bus, float assetDefault, float userMul = 1.0f) const;

    private:
        bool mInitialized{ false };

        SoundRepository mRepository;
        SoundCache mCache;

        float mMasterVolume{ 1.0f };
        float mSfxVolume{ 1.0f };
        float mMusicVolume{ 1.0f };
        float mUiVolume{ 1.0f };
    };
}


#endif //RAYMOB1_SOUNDMANAGER_H
