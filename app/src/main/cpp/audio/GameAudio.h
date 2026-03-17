//
// Created by jlhar on 3/16/2026.
//

#ifndef RAYMOB1_GAMEAUDIO_H
#define RAYMOB1_GAMEAUDIO_H

namespace sfx
{
    class SoundManager;
}

// Register all game audio definitions into the repository.
// This does NOT load the sounds yet.
// It only defines what audio assets exist.
void RegisterGameAudio(sfx::SoundManager& audio);

#endif //RAYMOB1_GAMEAUDIO_H
