//
// Created by jlhar on 3/16/2026.
//

#ifndef RAYMOB1_SOUNDREPOSITORY_H
#define RAYMOB1_SOUNDREPOSITORY_H



#include "AudioTypes.h"

#include <string>
#include <unordered_map>
#include <vector>

namespace sfx
{
    class SoundRepository
    {
    public:
        bool add(const AudioAssetDef& def);
        bool contains(const std::string& id) const;
        const AudioAssetDef* find(const std::string& id) const;

        std::vector<const AudioAssetDef*> findByTag(const std::string& tag) const;

        void clear();

    private:
        std::unordered_map<std::string, AudioAssetDef> mDefs;
    };
}


#endif //RAYMOB1_SOUNDREPOSITORY_H
