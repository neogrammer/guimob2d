//
// Created by jlhar on 3/16/2026.
//

#include "SoundCache.h"

namespace sfx
{
    SoundCache::~SoundCache()
    {
        unloadAll();
    }

    bool SoundCache::hasSfx(const std::string& id) const
    {
        return mSfx.find(id) != mSfx.end();
    }

    bool SoundCache::hasMusic(const std::string& id) const
    {
        return mMusic.find(id) != mMusic.end();
    }

    LoadedSfx* SoundCache::getSfx(const std::string& id)
    {
        auto it = mSfx.find(id);
        if (it == mSfx.end())
        {
            return nullptr;
        }

        return &it->second;
    }

    const std::unordered_map<std::string, LoadedMusic>& SoundCache::musicMap() const
    {
        return mMusic;
    }

    std::unordered_map<std::string, LoadedMusic>& SoundCache::musicMap()
    {
        return mMusic;
    }

    LoadedMusic* SoundCache::getMusic(const std::string& id)
    {
        auto it = mMusic.find(id);
        if (it == mMusic.end())
        {
            return nullptr;
        }

        return &it->second;
    }

    bool SoundCache::storeSfx(const std::string& id, const LoadedSfx& sfx)
    {
        if (id.empty())
        {
            return false;
        }

        if (hasSfx(id))
        {
            return false;
        }

        mSfx.emplace(id, sfx);
        return true;
    }

    bool SoundCache::storeMusic(const std::string& id, const LoadedMusic& music)
    {
        if (id.empty())
        {
            return false;
        }

        if (hasMusic(id))
        {
            return false;
        }

        mMusic.emplace(id, music);
        return true;
    }

    void SoundCache::markTagLoaded(const std::string& tag)
    {
        if (!tag.empty())
        {
            mLoadedTags.insert(tag);
        }
    }

    bool SoundCache::isTagLoaded(const std::string& tag) const
    {
        return mLoadedTags.find(tag) != mLoadedTags.end();
    }

    void SoundCache::unloadSfx(const std::string& id)
    {
        auto it = mSfx.find(id);
        if (it == mSfx.end())
        {
            return;
        }

        UnloadSound(it->second.Handle);
        mSfx.erase(it);
    }

    void SoundCache::unloadMusic(const std::string& id)
    {
        auto it = mMusic.find(id);
        if (it == mMusic.end())
        {
            return;
        }

        StopMusicStream(it->second.Handle);
        UnloadMusicStream(it->second.Handle);
        mMusic.erase(it);
    }

    void SoundCache::unloadAll()
    {
        for (auto& [id, sfx] : mSfx)
        {
            UnloadSound(sfx.Handle);
        }
        mSfx.clear();

        for (auto& [id, music] : mMusic)
        {
            StopMusicStream(music.Handle);
            UnloadMusicStream(music.Handle);
        }
        mMusic.clear();

        mLoadedTags.clear();
    }
}