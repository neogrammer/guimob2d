//
// Created by jlhar on 3/16/2026.
//


#include "GameAudio.h"

#include "SoundManager.h"
#include "AudioTypes.h"

void RegisterGameAudio(sfx::SoundManager& audio)
{
    using namespace sfx;

    // --------------------------------------------------
    // TITLE / UI
    // --------------------------------------------------
    {
        AudioAssetDef def{};
        def.Id = "menu_tap";
        def.Path = "audio/ui/menu_tap.wav";
        def.Type = AudioAssetType::Sfx;
        def.Bus = AudioBus::Ui;
        def.DefaultVolume = 0.85f;
        def.Looping = false;
        def.Tags = { "title", "ui" };
        audio.repository().add(def);
    }

    {
        AudioAssetDef def{};
        def.Id = "menu_confirm";
        def.Path = "audio/ui/menu_confirm.wav";
        def.Type = AudioAssetType::Sfx;
        def.Bus = AudioBus::Ui;
        def.DefaultVolume = 0.90f;
        def.Looping = false;
        def.Tags = { "title", "ui" };
        audio.repository().add(def);
    }

    {
        AudioAssetDef def{};
        def.Id = "title_music";
        def.Path = "audio/music/title_loop.ogg";
        def.Type = AudioAssetType::Music;
        def.Bus = AudioBus::Music;
        def.DefaultVolume = 1.0f;
        def.Looping = true;
        def.Tags = { "title" };
        audio.repository().add(def);
    }

    // --------------------------------------------------
    // GAMEPLAY
    // --------------------------------------------------
    {
        AudioAssetDef def{};
        def.Id = "footstep";
        def.Path = "audio/sfx/footstep.wav";
        def.Type = AudioAssetType::Sfx;
        def.Bus = AudioBus::Sfx;
        def.DefaultVolume = 0.70f;
        def.Looping = false;
        def.Tags = { "gameplay" };
        audio.repository().add(def);
    }

    {
        AudioAssetDef def{};
        def.Id = "pickup";
        def.Path = "audio/sfx/pickup.wav";
        def.Type = AudioAssetType::Sfx;
        def.Bus = AudioBus::Sfx;
        def.DefaultVolume = 0.85f;
        def.Looping = false;
        def.Tags = { "gameplay" };
        audio.repository().add(def);
    }

    {
        AudioAssetDef def{};
        def.Id = "drop";
        def.Path = "audio/sfx/drop.wav";
        def.Type = AudioAssetType::Sfx;
        def.Bus = AudioBus::Sfx;
        def.DefaultVolume = 0.80f;
        def.Looping = false;
        def.Tags = { "gameplay" };
        audio.repository().add(def);
    }

    {
        AudioAssetDef def{};
        def.Id = "bit_toggle";
        def.Path = "audio/sfx/bit_toggle.wav";
        def.Type = AudioAssetType::Sfx;
        def.Bus = AudioBus::Sfx;
        def.DefaultVolume = 0.75f;
        def.Looping = false;
        def.Tags = { "gameplay" };
        audio.repository().add(def);
    }

    {
        AudioAssetDef def{};
        def.Id = "combine";
        def.Path = "audio/sfx/combine.wav";
        def.Type = AudioAssetType::Sfx;
        def.Bus = AudioBus::Sfx;
        def.DefaultVolume = 0.90f;
        def.Looping = false;
        def.Tags = { "gameplay" };
        audio.repository().add(def);
    }

    {
        AudioAssetDef def{};
        def.Id = "teleport";
        def.Path = "audio/sfx/teleport.wav";
        def.Type = AudioAssetType::Sfx;
        def.Bus = AudioBus::Sfx;
        def.DefaultVolume = 0.95f;
        def.Looping = false;
        def.Tags = { "gameplay" };
        audio.repository().add(def);
    }

    {
        AudioAssetDef def{};
        def.Id = "error_buzz";
        def.Path = "audio/sfx/error_buzz.wav";
        def.Type = AudioAssetType::Sfx;
        def.Bus = AudioBus::Sfx;
        def.DefaultVolume = 0.85f;
        def.Looping = false;
        def.Tags = { "gameplay", "ui" };
        audio.repository().add(def);
    }

    {
        AudioAssetDef def{};
        def.Id = "submit_ok";
        def.Path = "audio/sfx/submit_ok.wav";
        def.Type = AudioAssetType::Sfx;
        def.Bus = AudioBus::Sfx;
        def.DefaultVolume = 1.00f;
        def.Looping = false;
        def.Tags = { "gameplay" };
        audio.repository().add(def);
    }

    {
        AudioAssetDef def{};
        def.Id = "submit_bad";
        def.Path = "audio/sfx/submit_bad.wav";
        def.Type = AudioAssetType::Sfx;
        def.Bus = AudioBus::Sfx;
        def.DefaultVolume = 0.90f;
        def.Looping = false;
        def.Tags = { "gameplay" };
        audio.repository().add(def);
    }

    {
        AudioAssetDef def{};
        def.Id = "gameplay_music";
        def.Path = "audio/music/gameplay_loop.ogg";
        def.Type = AudioAssetType::Music;
        def.Bus = AudioBus::Music;
        def.DefaultVolume = 1.0f;
        def.Looping = true;
        def.Tags = { "gameplay" };
        audio.repository().add(def);
    }
}