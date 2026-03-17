//
// Created by jlhar on 3/16/2026.
//

#ifndef RAYMOB1_SOUNDCACHE_H
#define RAYMOB1_SOUNDCACHE_H


#include "AudioTypes.h"
#include "raylib.h"

#include <string>
#include <unordered_map>
#include <unordered_set>

namespace sfx
{
    struct LoadedSfx
    {
        Sound Handle{};
        float DefaultVolume{ 1.0f };
        AudioBus Bus{ AudioBus::Sfx };
    };

    struct LoadedMusic
    {
        Music Handle{};
        float DefaultVolume{ 1.0f };
        AudioBus Bus{ AudioBus::Music };
        bool Looping{ true };
    };

    class SoundCache
    {
    public:
        ~SoundCache();

        bool hasSfx(const std::string& id) const;
        bool hasMusic(const std::string& id) const;

        LoadedSfx* getSfx(const std::string& id);
        LoadedMusic* getMusic(const std::string& id);

        bool storeSfx(const std::string& id, const LoadedSfx& sfx);
        bool storeMusic(const std::string& id, const LoadedMusic& music);

        void markTagLoaded(const std::string& tag);
        bool isTagLoaded(const std::string& tag) const;

        void unloadSfx(const std::string& id);
        void unloadMusic(const std::string& id);

        void unloadAll();

        const std::unordered_map<std::string, LoadedMusic>& musicMap() const;
        std::unordered_map<std::string, LoadedMusic>& musicMap();

    private:
        std::unordered_map<std::string, LoadedSfx> mSfx;
        std::unordered_map<std::string, LoadedMusic> mMusic;
        std::unordered_set<std::string> mLoadedTags;


    };
}

#endif //RAYMOB1_SOUNDCACHE_H
