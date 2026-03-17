//
// Created by jlhar on 3/16/2026.
//
#include "SoundRepository.h"

#include <algorithm>

namespace sfx
{
    bool sfx::SoundRepository::add(const AudioAssetDef& def)
    {
        // Reject invalid IDs up front.
        if (def.Id.empty())
        {
            return false;
        }

        // Reject duplicates.
        if (mDefs.find(def.Id) != mDefs.end())
        {
            return false;
        }

        mDefs.emplace(def.Id, def);
        return true;
    }

    bool sfx::SoundRepository::contains(const std::string& id) const
    {
        return mDefs.find(id) != mDefs.end();
    }

    const sfx::AudioAssetDef* sfx::SoundRepository::find(const std::string& id) const
    {
        auto it = mDefs.find(id);
        if (it == mDefs.end())
        {
            return nullptr;
        }

        return &it->second;
    }

    std::vector<const sfx::AudioAssetDef*> sfx::SoundRepository::findByTag(const std::string& tag) const
    {
        std::vector<const AudioAssetDef*> results;

        if (tag.empty())
        {
            return results;
        }

        results.reserve(mDefs.size());

        for (const auto& [id, def] : mDefs)
        {
            auto tagIt = std::find(def.Tags.begin(), def.Tags.end(), tag);
            if (tagIt != def.Tags.end())
            {
                results.push_back(&def);
            }
        }

        return results;
    }

    void SoundRepository::clear()
    {
        mDefs.clear();
    }
}