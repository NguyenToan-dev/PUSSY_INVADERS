#include "MusicController.h"
#include<iostream>
#include<cstring>

MusicController::MusicController()
{
    musicState = MUSIC_IDLE;
    musicLoaded = false;
    masterVolume = 0.5f;
    currentVolume = 0.0f; 
    fadeDuration = 2.5f;
    pausedVolume = 0.0f;
    LoadBackgroundMusic();
}

MusicController::~MusicController() {}

void MusicController::LoadBackgroundMusic() 
{ 
    backgroundMusic = LoadMusicStream("sound/background_music.mp3");
    if (backgroundMusic.stream.buffer != nullptr) 
    {
        musicLoaded = true;
        std::cout << "Successfully loaded music" << std::endl;
        return;
    }
    std::cout << "Warning: Could not load any background music files.\n";
}

void MusicController::HandleMusic(GameState gameState) 
{
    if (!musicLoaded || gameState == GAME_GAME_OVER) 
        return;

    UpdateMusicStream(backgroundMusic);

    switch(musicState) 
    {
        case MUSIC_FADING_IN: 
        {
            currentVolume += (pausedVolume > 0.0f ? pausedVolume : masterVolume) / fadeDuration * GetFrameTime();
            if (currentVolume >= (pausedVolume > 0.0f ? pausedVolume : masterVolume)) 
            {
                currentVolume = pausedVolume > 0.0f ? pausedVolume : masterVolume;
                musicState = MUSIC_PLAYING;
            }
            SetMusicVolume(backgroundMusic, currentVolume);
            break;
        }
        case MUSIC_PLAYING: 
        {
            float musicLength = GetMusicTimeLength(backgroundMusic);
            float timePlayed = GetMusicTimePlayed(backgroundMusic);
            if (musicLength > 0 && (musicLength - timePlayed) <= fadeDuration) 
                musicState = MUSIC_FADING_OUT;
            break;
        }
        case MUSIC_FADING_OUT: 
        {
            currentVolume -= (masterVolume / fadeDuration) * GetFrameTime();
            if (currentVolume <= 0.0f) 
            {
                currentVolume = 0.0f;
                musicState = MUSIC_IDLE;
                StopMusicStream(backgroundMusic);
                PlayMusicStream(backgroundMusic);
                musicState = MUSIC_FADING_IN;
                SetMusicVolume(backgroundMusic, currentVolume);
            }
            else 
                SetMusicVolume(backgroundMusic, currentVolume);
            break;
        }
        case MUSIC_IDLE:
        case MUSIC_PAUSED:
            break;
    }
}

bool MusicController::IsMusicLoaded() const { return musicLoaded; }

Music MusicController::GetMusic() const { return backgroundMusic; }

void MusicController::SetState(MusicState data)
{
    musicState = data;
    return;
}

MusicState MusicController::GetState() const { return musicState; }

void MusicController::SetMVolume(float data)
{
    masterVolume = data;
    return;
}

float MusicController::GetMVolume() const { return masterVolume; }

void MusicController::SetCVolume(float data)
{
    currentVolume = data;
    return;
}

void MusicController::SetDuration(float data)
{
    fadeDuration = data;
    return;
}

void MusicController::SetPVolume(float data)
{
    pausedVolume = data;
    return;
}

float MusicController::GetDuration() const { return fadeDuration; }

float MusicController::GetPVolume() const { return pausedVolume; }

float MusicController::GetCVolume() const { return currentVolume; }

void MusicController::SetVolumeNow(float volume) 
{
    masterVolume = volume;
    currentVolume = masterVolume;
    SetMusicVolume(backgroundMusic, currentVolume);
    musicState = MUSIC_PLAYING;
    std::cout << "Volume set to: " << (int)(masterVolume * 100) << "%\n";
}