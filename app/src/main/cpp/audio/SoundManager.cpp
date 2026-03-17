//
// Created by jlhar on 3/16/2026.
//

#include "SoundManager.h"

#include <algorithm>

namespace sfx
{
    namespace
    {
        float Clamp01(float v)
        {
            return std::max(0.0f, std::min(1.0f, v));
        }
    }

    bool SoundManager::init()
    {
        if (mInitialized)
        {
            return true;
        }

        InitAudioDevice();
        mInitialized = IsAudioDeviceReady();
        return mInitialized;
    }

    void SoundManager::shutdown()
    {
        if (!mInitialized)
        {
            return;
        }

        stopAllMusic();
        mCache.unloadAll();

        CloseAudioDevice();
        mInitialized = false;
    }

    void SoundManager::update()
    {

        if (!mInitialized)
        {
            return;
        }
        for (auto& [id, music] : mCache.musicMap())
        {
            UpdateMusicStream(music.Handle);

            const float vol = effectiveVolume(music.Bus, music.DefaultVolume);
            SetMusicVolume(music.Handle, vol);
        }
    }

    SoundRepository& SoundManager::repository()
    {
        return mRepository;
    }

    const SoundRepository& SoundManager::repository() const
    {
        return mRepository;
    }

    bool SoundManager::loadAsset(const std::string& id)
    {
        if (!mInitialized)
        {
            return false;
        }

        const AudioAssetDef* def = mRepository.find(id);
        if (!def)
        {
            return false;
        }

        if (def->Type == AudioAssetType::Sfx)
        {
            if (mCache.hasSfx(id))
            {
                return true;
            }

            Sound sfx = LoadSound(def->Path.c_str());
            if (sfx.frameCount == 0)
            {
                return false;
            }

            LoadedSfx loaded{};
            loaded.Handle = sfx;
            loaded.DefaultVolume = def->DefaultVolume;
            loaded.Bus = def->Bus;

            const bool stored = mCache.storeSfx(id, loaded);
            if (!stored)
            {
                UnloadSound(sfx);
                return false;
            }

            SetSoundVolume(loaded.Handle, effectiveVolume(loaded.Bus, loaded.DefaultVolume));
            return true;
        }

        if (def->Type == AudioAssetType::Music)
        {
            if (mCache.hasMusic(id))
            {
                return true;
            }

            Music music = LoadMusicStream(def->Path.c_str());
            if (music.ctxData == nullptr)
            {
                return false;
            }

            LoadedMusic loaded{};
            loaded.Handle = music;
            loaded.DefaultVolume = def->DefaultVolume;
            loaded.Bus = def->Bus;
            loaded.Looping = def->Looping;

            const bool stored = mCache.storeMusic(id, loaded);
            if (!stored)
            {
                UnloadMusicStream(music);
                return false;
            }

            SetMusicVolume(loaded.Handle, effectiveVolume(loaded.Bus, loaded.DefaultVolume));
            loaded.Handle.looping = def->Looping;
            return true;
        }

        return false;
    }

    bool SoundManager::loadTag(const std::string& tag)
    {
        if (!mInitialized)
        {
            return false;
        }

        if (tag.empty())
        {
            return false;
        }

        if (mCache.isTagLoaded(tag))
        {
            return true;
        }

        const auto defs = mRepository.findByTag(tag);
        bool allLoaded = true;

        for (const AudioAssetDef* def : defs)
        {
            if (!def)
            {
                allLoaded = false;
                continue;
            }

            if (!loadAsset(def->Id))
            {
                allLoaded = false;
            }
        }

        if (allLoaded)
        {
            mCache.markTagLoaded(tag);
        }

        return allLoaded;
    }

    void SoundManager::unloadAsset(const std::string& id)
    {
        if (id.empty())
        {
            return;
        }

        if (mCache.hasSfx(id))
        {
            mCache.unloadSfx(id);
            return;
        }

        if (mCache.hasMusic(id))
        {
            mCache.unloadMusic(id);
            return;
        }
    }

    void SoundManager::unloadAll()
    {
        mCache.unloadAll();
    }

    void SoundManager::playSfx(const std::string& id, float volumeMultiplier)
    {
        if (!mInitialized)
        {
            return;
        }

        if (!mCache.hasSfx(id))
        {
            if (!loadAsset(id))
            {
                return;
            }
        }

        LoadedSfx* sfx = mCache.getSfx(id);
        if (!sfx)
        {
            return;
        }

        const float vol = effectiveVolume(sfx->Bus, sfx->DefaultVolume, volumeMultiplier);
        SetSoundVolume(sfx->Handle, vol);
        PlaySound(sfx->Handle);
    }

    void SoundManager::playMusic(const std::string& id, bool restartIfPlaying)
    {
        if (!mInitialized)
        {
            return;
        }

        if (!mCache.hasMusic(id))
        {
            if (!loadAsset(id))
            {
                return;
            }
        }

        LoadedMusic* music = mCache.getMusic(id);
        if (!music)
        {
            return;
        }

        music->Handle.looping = music->Looping;
        SetMusicVolume(music->Handle, effectiveVolume(music->Bus, music->DefaultVolume));

        if (IsMusicStreamPlaying(music->Handle))
        {
            if (restartIfPlaying)
            {
                StopMusicStream(music->Handle);
                PlayMusicStream(music->Handle);
            }
            return;
        }

        PlayMusicStream(music->Handle);
    }

    void SoundManager::stopMusic(const std::string& id)
    {
        if (!mInitialized)
        {
            return;
        }

        LoadedMusic* music = mCache.getMusic(id);
        if (!music)
        {
            return;
        }

        StopMusicStream(music->Handle);
    }

    void SoundManager::stopAllMusic()
    {
        if (!mInitialized)
        {
            return;
        }

        for (auto& [id, music] : mCache.musicMap())
        {
            StopMusicStream(music.Handle);
        }
    }

    void SoundManager::setMasterVolume(float v)
    {
        mMasterVolume = Clamp01(v);
    }

    void SoundManager::setBusVolume(AudioBus bus, float v)
    {
        const float clamped = Clamp01(v);

        switch (bus)
        {
            case AudioBus::Master:
                mMasterVolume = clamped;
                break;
            case AudioBus::Sfx:
                mSfxVolume = clamped;
                break;
            case AudioBus::Music:
                mMusicVolume = clamped;
                break;
            case AudioBus::Ui:
                mUiVolume = clamped;
                break;
            default:
                break;
        }
    }

    float SoundManager::masterVolume() const
    {
        return mMasterVolume;
    }

    float SoundManager::busVolume(AudioBus bus) const
    {
        switch (bus)
        {
            case AudioBus::Master: return mMasterVolume;
            case AudioBus::Sfx:    return mSfxVolume;
            case AudioBus::Music:  return mMusicVolume;
            case AudioBus::Ui:     return mUiVolume;
            default:               return 1.0f;
        }
    }

    float SoundManager::effectiveVolume(AudioBus bus, float assetDefault, float userMul) const
    {
        const float base = Clamp01(assetDefault) * Clamp01(userMul) * mMasterVolume;

        switch (bus)
        {
            case AudioBus::Master:
                return Clamp01(base);
            case AudioBus::Sfx:
                return Clamp01(base * mSfxVolume);
            case AudioBus::Music:
                return Clamp01(base * mMusicVolume);
            case AudioBus::Ui:
                return Clamp01(base * mUiVolume);
            default:
                return Clamp01(base);
        }
    }
}