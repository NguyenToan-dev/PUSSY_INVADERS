#pragma once
#include "Addition.h"
#include<raylib.h>

class MusicController
{
private:
    MusicState musicState;
    Music backgroundMusic;
    bool musicLoaded;
    float masterVolume;
    float currentVolume;
    float fadeDuration;
    float pausedVolume;

public:
    MusicController();
    ~MusicController();
    void LoadBackgroundMusic();
    void HandleMusic(GameState);
    bool IsMusicLoaded() const;
    Music GetMusic() const;
    
    MusicState GetState() const;
    void SetState(MusicState);
    void SetMVolume(float);
    float GetMVolume() const;
    void SetCVolume(float);
    float GetCVolume() const;
    
    float GetPVolume() const;
    void SetPVolume(float);
    float GetDuration() const;
    void SetDuration(float);

    void SetVolumeNow(float);
};


