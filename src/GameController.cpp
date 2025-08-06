//GameController.cpp
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

    // recently added
    ship.SetAttribute();
    ship_shootsound = LoadSound("sound/lasergun.wav");
    pussy_shootsound = LoadSound("sound/bom.mp3");
    SetSoundVolume(ship_shootsound, 1.0f);
    SetSoundVolume(pussy_shootsound, .4f);
    bullets.reserve(20);
    bullet_texture = LoadTexture("image/bullet.png");
    Pussy::LoadImage();
    Pussy2::LoadImage();
    Pussy3::LoadImage();
    pussies.reserve(18);
    pussy_shit_texture = LoadTexture("image/shit.png");
    thunder_texture = LoadTexture("image/thunder.png"); 
    thunderSound = LoadSound("sound/thunder.mp3"); 
    SetSoundVolume(thunderSound, .8f); 
    for (int row = 0; row < 3; row++) 
    {
        for (int i = 0; i < 6; i++) 
        {
            Vector2 pos = { 150.0f + i * 200, 100.0f + row * 100 };
            pussies.push_back(new Pussy(1, pos)); // Sửa lỗi: Tạo Pussy thay vì PussyBase
        }
    }
    isblinking = false;
    blinkspeed = 10.f;
    blinkduration = 2.f;
    blink1 = { 0, 0, 0, 100 };   // Semi-transparent black (R, G, B, Alpha)
    blink2 = { 255, 255, 255, 100 }; // Semi-transparent white
}

GameController::~GameController()
{
    UnloadTexture(background.GetTexture());
    UnloadMusicStream(music.GetMusic());
    
    for (auto* pussy : pussies) {
        delete pussy;}
    pussies.clear();
    
    for (auto* bullet : pussyBullets) {
        delete bullet;}
    pussyBullets.clear();

    // recently added
    UnloadTexture(bullet_texture);
    UnloadSound(ship_shootsound);
    UnloadSound(pussy_shootsound);
    Pussy::UnloadImage();
    Pussy2::UnloadImage();
    Pussy3::UnloadImage();
    UnloadTexture(thunder_texture);
    UnloadTexture(pussy_shit_texture);
    UnloadSound(thunderSound);
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
    //////////////////////////////////
    Pickup::UpdateAll(GetFrameTime());//->Cập nhật tất cả pickups mỗi frame
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


void GameController::HandlePussyWaveProgression()
{
    if (pussies.empty())
    {
        if (currentPussyStage == 1)
        {
            currentPussyStage = 2;
            for (int row = 0; row < 3; row++)
            {
                for (int i = 0; i < 6; i++)
                {
                    Vector2 pos = { 150.0f + i * 200, 100.0f + row * 100 };
                    pussies.push_back(new Pussy2(1, pos));
                }
            }
        }
        else if (currentPussyStage == 2)
        {
            currentPussyStage = 3;
            for (int row = 0; row < 3; row++)
            {
                for (int i = 0; i < 6; i++)
                {
                    Vector2 pos = { 150.0f + i * 200, 100.0f + row * 100 };
                    pussies.push_back(new Pussy3(1, pos));
                }
            }
        }
        else if (currentPussyStage == 3)
        {
            currentPussyStage = 4;
        }
    }
}

void GameController::HandleObjectDrawing()
{
    background.DrawRotatingBackground();
    
    for (auto* pussy : pussies)
        pussy->Draw();

    double elapsedTime = GetTime() - timestart;
    Color currentOverlayColor;
    if (elapsedTime >= blinkduration)
        isblinking = false;
    else 
    {
        if (isblinking)
            currentOverlayColor = ((int)(elapsedTime * blinkspeed) % 2 == 0) ? blink1 : blink2;
    }

    if (isblinking)
        goto Skip_Ship_Bullets;

    if (IsKeyPressed(KEY_SPACE))
        ship.Shooting(bullets, &bullet_texture), PlaySound(ship_shootsound);

    for (int i = 0; i < (int)bullets.size(); i++)
    {
        if (bullets[i].active == false)
        {
            bullets.erase(bullets.begin() + i);
            i--;
            continue;
        }
        bullets[i].Update();
        bullets[i].Draw();
    }

    for (auto& bullet : bullets) 
    {
        if (!bullet.active) 
            continue;
        for (auto* pussy : pussies)
        {
            if (CheckCollisionRecs(bullet.getRect(), pussy->getRect()))
            {
                bullet.active = false;
                pussy->health--;
                if (pussy->health <= 0)
                {
                    Rectangle r = pussy->getRect();
                    Vector2 spawnPos = { r.x + r.width / 2, r.y + r.height / 2 };
                    Pickup::Spawn(spawnPos);
                    pussy->position.x = -9999; // Đánh dấu để xóa
                    ship.AdjustStatus(SCORE_GAIN_1);
                }
                break;
            }
        }
    }

    for (int i = 0; i < (int)pussies.size(); i++)
    {
        if (pussies[i]->position.x < 0)
        {
            delete pussies[i];
            pussies.erase(pussies.begin() + i);
            i--;
        }
    }

    HandlePussyWaveProgression();

Skip_Ship_Bullets:

    bool reachEdge = false;
    for (auto* pussy : pussies) 
    {
        pussy->Update(PussyBase::pussyDirection);
        Rectangle r = pussy->getRect();
        if (r.x < 0 || r.x + r.width > GetScreenWidth()) 
            reachEdge = true;
    }
    if (reachEdge) 
    {
        PussyBase::pussyDirection = -(PussyBase::pussyDirection);
        for (auto* pussy : pussies) 
            pussy->position.y += 20;
    }

    PussyBase::pussyShootTimer += GetFrameTime();
    if (PussyBase::pussyShootTimer >= PussyBase::pussyShootInterval && !pussies.empty()) 
    {
        PussyBase::pussyShootTimer = 0.0f;
        int idx = GetRandomValue(0, pussies.size() - 1);
        auto* shooter = pussies[idx];
        Vector2 pos = {
            shooter->getRect().x + shooter->getRect().width / 2,
            shooter->getRect().y + shooter->getRect().height
        };

        int randType = GetRandomValue(1, 3);
        Bullet* newBullet;
        if (randType == 1) {
            newBullet = new ThunderBullet(pos, &thunder_texture);
            PlaySound(thunderSound);
        } else {
            newBullet = new Bullet(pos, &pussy_shit_texture);
            newBullet->speed = -newBullet->speed;
        }

        pussyBullets.push_back(newBullet);
        PlaySound(pussy_shootsound);
    }   

    for (int i = 0; i < (int)pussyBullets.size(); i++) {
        if (!pussyBullets[i]->active) {
            delete pussyBullets[i];
            pussyBullets.erase(pussyBullets.begin() + i);
            i--;
            continue;
        }
        pussyBullets[i]->Update();
        pussyBullets[i]->Draw();
    }

    if (!isblinking)
    {
        int flag = ship.HitBoxChecking(pussyBullets);
        if (flag == 2)
        {
            gameState = GAME_GAME_OVER;
            return;
        }
        else if (flag == 1)
        {
            timestart = GetTime();
            isblinking = true;
            ship.StatusBar();
            return;
        }
    }
    
    if (isblinking)
        ship.MovingWhileBlinking(currentOverlayColor);
    else
        ship.Moving();
    ship.EatPickup();

    ship.StatusBar();
}
void GameController::Draw() 
{
    HandleObjectDrawing();

    if (gameState == GAME_GAME_OVER)
        DrawGameOver();    
    else if (gameState == GAME_COUNTDOWN)
        DrawCountdown();   
    else if (gameState == GAME_PAUSED)
        DrawPaused();   
    //else DrawUI();

    Pickup::DrawAll();//recently added
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

