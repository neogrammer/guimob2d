//
// Created by jlhar on 3/16/2026.
//

#ifndef RAYMOB1_AUDIOTYPES_H
#define RAYMOB1_AUDIOTYPES_H

#include <string>
#include <vector>

namespace sfx {

    enum class AudioAssetType {
        Sfx,
        Music
    };

    enum class AudioBus {
        Master,
        Sfx,
        Music,
        Ui
    };

    struct AudioAssetDef {
        std::string Id;
        std::string Path;
        AudioAssetType Type{AudioAssetType::Sfx};
        AudioBus Bus{AudioBus::Sfx};
        float DefaultVolume{1.0f};
        bool Looping{false};

        // arbitrary grouping tags like "title", "gameplay", "level1"
        std::vector<std::string> Tags;
    };

}

#endif //RAYMOB1_AUDIOTYPES_H
