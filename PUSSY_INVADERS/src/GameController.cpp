#include "GameController.h"
#include<raylib.h>
#include<iostream>

GameController::GameController()
{
    background = Background();
    music = MusicController();
    gameState = GAME_PLAYING_STATE;
    gameover = GameOver();   
    blurOpacity = 0.0f; 
    countdownTimer = 0.0f;

    ship.SetAttribute();
    bullets.reserve(20);
    bullet_texture = LoadTexture("image/bullet.png");
    Pussy::LoadImage();
    pussies.reserve(30);
}

GameController::~GameController()
{
    UnloadTexture(background.GetTexture());
    UnloadMusicStream(music.GetMusic());
    
    UnloadTexture(bullet_texture);
    Pussy::UnloadImage();
}

void GameController::Update() 
{
    if (gameState == GAME_PLAYING_STATE) 
    {
        background.SetRotation(background.GetRotation() + background.GetSpeed() * GetFrameTime());
        music.HandleMusic(gameState);
        HandleInput();
    }
    else if (gameState == GAME_GAME_OVER) 
    {
        if (gameover.GetPhase() == FREEZE || gameover.GetPhase() == FADE_BLUR) 
            background.SetRotation(background.GetRotation() + background.GetSpeed() * 
                GetFrameTime() * SLOW_MOTION_FACTOR);
        gameover.HandleGameOver(blurOpacity);
        if (gameover.GetPhase() == TEXT_DISPLAY && IsKeyPressed(KEY_R)) 
            HandleInput();
    }
    else if (gameState == GAME_COUNTDOWN) 
    {
        float speedFactor = SLOW_MOTION_FACTOR + 
            (1.0f - SLOW_MOTION_FACTOR) * (countdownTimer / COUNTDOWN_DURATION);
        background.SetRotation(background.GetRotation() + background.GetSpeed() * GetFrameTime() * speedFactor);
        HandleCountdown();
        music.HandleMusic(gameState);
    }
    else if (gameState == GAME_PAUSED) 
    {
        if (music.IsMusicLoaded()) 
            UpdateMusicStream(music.GetMusic());
        HandleInput();
    }
    if (IsWindowResized()) 
    {
        background.SetCenter((float)GetScreenWidth() / 2.0f, (float)GetScreenHeight() / 2.0f);
        background.CalculateBackgroundScale();
    }
}

void GameController::HandleCountdown() 
{
    countdownTimer += GetFrameTime();
    if (countdownTimer >= COUNTDOWN_DURATION) 
    {
        gameState = GAME_PLAYING_STATE;
        gameover.SetPhase(FREEZE);
        gameover.SetTime(.0f);
        blurOpacity = 0.0f;
        std::cout << "Game resumed\n";
    }
}

void GameController::HandleInput() 
{
    MusicState state = music.GetState();
    float mvolume = music.GetMVolume();

    if (gameState == GAME_PLAYING_STATE) 
    {
        if (IsKeyPressed(KEY_M) && music.IsMusicLoaded()) 
        {
            if (state == MUSIC_PLAYING || state == MUSIC_FADING_IN) 
                music.SetState(MUSIC_FADING_OUT);
            
            else if (state == MUSIC_IDLE) 
            {
                music.SetState(MUSIC_FADING_IN);
                PlayMusicStream(music.GetMusic());
            }
        }
        if (IsKeyPressed(KEY_EQUAL) || IsKeyPressed(KEY_KP_ADD)) 
        {
            music.SetMVolume(std::min(1.0f, mvolume + 0.1f));
            mvolume = music.GetMVolume();
            music.SetVolumeNow(mvolume);
        }
        if (IsKeyPressed(KEY_MINUS) || IsKeyPressed(KEY_KP_SUBTRACT)) 
        {
            music.SetMVolume(std::max(0.0f, mvolume - 0.1f));
            mvolume = music.GetMVolume();
            music.SetVolumeNow(mvolume);
        }
        if (IsKeyPressed(KEY_G)) 
        {
            gameState = GAME_GAME_OVER;
            gameover.SetPhase(FREEZE);
            gameover.SetTime(0.0f);
            blurOpacity = 0.0f;
            if (music.IsMusicLoaded()) 
            {
                StopMusicStream(music.GetMusic());
                music.SetState(MUSIC_IDLE);
                music.SetCVolume(0.0f);
                SetMusicVolume(music.GetMusic(), music.GetCVolume());
                std::cout << "Music stopped\n";
            }
            std::cout << "Game Over triggered\n";
        }
        if (IsKeyPressed(KEY_P)) 
        {
            gameState = GAME_PAUSED;
            if (music.IsMusicLoaded()) 
            {
                music.SetPVolume(music.GetCVolume());
                PauseMusicStream(music.GetMusic());
                music.SetState(MUSIC_PAUSED);
                music.SetCVolume(0.0f);
                SetMusicVolume(music.GetMusic(), music.GetCVolume());
                std::cout << "Music paused\n";
            }
            std::cout << "Game paused\n";
        }
        if (IsKeyPressed(KEY_F)) 
        {
            ToggleFullscreen();
            background.SetCenter((float)GetScreenWidth() / 2.0f, (float)GetScreenHeight() / 2.0f);
            background.CalculateBackgroundScale();
            std::cout << "Toggled fullscreen\n";
        }
    }
    else if (gameState == GAME_PAUSED) 
    {
        if (IsKeyPressed(KEY_P)) 
        {
            gameState = GAME_COUNTDOWN;
            countdownTimer = 0.0f;
            if (music.IsMusicLoaded()) 
            {
                if (!IsMusicStreamPlaying(music.GetMusic())) 
                {
                    PlayMusicStream(music.GetMusic());
                    UpdateMusicStream(music.GetMusic());
                }
                ResumeMusicStream(music.GetMusic());
                music.SetCVolume(0.0f);
                SetMusicVolume(music.GetMusic(), music.GetCVolume());
                music.SetState(MUSIC_FADING_IN);
                std::cout << "Music resumed with fade-in\n";
            }
            std::cout << "Countdown started\n";
        }
    }
    else if (gameState == GAME_GAME_OVER && gameover.GetPhase() == TEXT_DISPLAY) 
    {
        if (IsKeyPressed(KEY_R)) 
        {
            gameState = GAME_COUNTDOWN;
            countdownTimer = 0.0f;
            if (music.IsMusicLoaded()) 
            {
                PlayMusicStream(music.GetMusic());
                music.SetState(MUSIC_FADING_IN);
                music.SetCVolume(0.0f);
                SetMusicVolume(music.GetMusic(), music.GetCVolume());
                std::cout << "Music resumed\n";
            }
            std::cout << "Countdown started\n";
        }
    }
}
    
void GameController::Draw() 
{
    background.DrawRotatingBackground();
    
    if(IsKeyPressed(KEY_SPACE))
        ship.Shooting(bullets, &bullet_texture);
    for(int i=0; i<(int)bullets.size(); i++)
    {
        if(bullets[i].active == false)
        {
            bullets.erase(bullets.begin() + i);
            i--;
            continue;
        }
        bullets[i].Update();
        bullets[i].Draw();
    }
    ship.Moving();
    //ship.StatusBar();
    
    if (gameState == GAME_GAME_OVER) {
        DrawGameOver();    
    }
    else if (gameState == GAME_COUNTDOWN)
        DrawCountdown();   
    else if (gameState == GAME_PAUSED)
        DrawPaused();   
    else 
        DrawUI();
}

void GameController::DrawGameOver() 
{
    float width = background.GetTexture().width;
    float height = background.GetTexture().height;
    float x = background.GetCenter().x;
    float y = background.GetCenter().y;
    float scale = background.GetScale();
    float rotation = background.GetRotation();

    DrawTexturePro(background.GetTexture(),
        { 0, 0, width, height },
        { x, y, width * scale, height * scale },
        { (width * scale) * 0.5f, (height * scale) * 0.5f },
        rotation, { 255, 255, 255, 255 });
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), 
        { 0, 0, 0, (unsigned char)std::min(blurOpacity, (float)MAX_BLUR_OPACITY) });

    switch (gameover.GetPhase()) 
    {
        case FREEZE:
        case FADE_BLUR:
            break;

        case RED_FLASH:
            DrawRectangleLinesEx({ 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() }, 
                10, { 255, 0, 0, 200 });
            break;

        case TEXT_DISPLAY:
            const char* gameOverText = "GAME OVER";
            int fontSize = 80;
            int textWidth = MeasureText(gameOverText, fontSize);
            DrawText(gameOverText, (GetScreenWidth() - textWidth) / 2, 
                GetScreenHeight() / 2 - fontSize / 2, fontSize, RED);
            const char* restartText = "Press R to Restart";
            int restartFontSize = 30;
            int restartTextWidth = MeasureText(restartText, restartFontSize);
            DrawText(restartText, (GetScreenWidth() - restartTextWidth) / 2, 
                GetScreenHeight() / 2 + fontSize, restartFontSize, WHITE);
            break;
    }
}
    
void GameController::DrawCountdown() 
{
    background.DrawRotatingBackground();
    float fadeOutOpacity = MAX_BLUR_OPACITY * (1.0f - countdownTimer / COUNTDOWN_DURATION);
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), { 0, 0, 0, (unsigned char)fadeOutOpacity });
    int count = 3 - static_cast<int>(countdownTimer);
    if (count >= 1) 
    {
        const char* countText = TextFormat("%d", count);
        int fontSize = 100;
        int textWidth = MeasureText(countText, fontSize);
        DrawText(countText, (GetScreenWidth() - textWidth) / 2,
            GetScreenHeight() / 2 - fontSize / 2, fontSize, WHITE);
    }
}

void GameController::DrawPaused() 
{
    background.DrawRotatingBackground();
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), { 0, 0, 0, 150 });
    const char* pauseText = "PAUSED";
    int fontSize = 80;
    int textWidth = MeasureText(pauseText, fontSize);
    DrawText(pauseText, (GetScreenWidth() - textWidth) / 2,
        GetScreenHeight() / 2 - fontSize / 2, fontSize, WHITE);
    const char* resumeText = "Press P to Resume";
    int resumeFontSize = 30;
    int resumeTextWidth = MeasureText(resumeText, resumeFontSize);
    DrawText(resumeText, (GetScreenWidth() - resumeTextWidth) / 2, 
        GetScreenHeight() / 2 + fontSize, resumeFontSize, WHITE);
}

void GameController::DrawUI() 
{
    DrawText("Pussy Invaders", 50, 50, 40, WHITE);
    DrawFPS(GetScreenWidth() - 100, 10);
    DrawText("Music Controls:", 10, 100, 20, WHITE);
    DrawText("M - Pause/Resume Music", 10, 125, 16, RAYWHITE);
    DrawText("+/- : Volume Up/Down", 10, 145, 16, RAYWHITE);
    DrawText("G - Trigger Game Over (Test)", 10, 165, 16, RAYWHITE);
    DrawText("P - Pause Game", 10, 185, 16, RAYWHITE);
    DrawText("F - Toggle Fullscreen", 10, 205, 16, RAYWHITE);
    if (music.IsMusicLoaded()) 
    {
        const char* statusText = "Idle";
        switch (music.GetState()) 
        {
            case MUSIC_PLAYING: statusText = "Playing"; break;
            case MUSIC_FADING_IN: statusText = "Fading In"; break;
            case MUSIC_FADING_OUT: statusText = "Fading Out"; break;
            case MUSIC_IDLE: statusText = (gameState == GAME_GAME_OVER) ? "Stopped" : "Idle"; break;
            case MUSIC_PAUSED: statusText = "Paused"; break;
        }
        DrawText(TextFormat("Music: %s (Vol: %.0f%%)", statusText, music.GetCVolume() * 100), 10, 230, 16, LIME);
    }
    else 
        DrawText("Music: Not Loaded", 10, 230, 16, RED);
}

