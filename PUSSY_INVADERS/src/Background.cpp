#include "raylib.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>

class Game {
private:
    // Chỉnh size window nhé ae
    static constexpr int SCREEN_WIDTH  = 1920;
    static constexpr int SCREEN_HEIGHT = 1080;

    // Screen and Background
    Texture2D backgroundTexture;
    float backgroundRotation;
    Vector2 backgroundCenter;
    float rotationSpeed;
    float backgroundScale;

    // --- Audio State Machine ---
    enum MusicState {
        MUSIC_IDLE,
        MUSIC_FADING_IN,
        MUSIC_PLAYING,
        MUSIC_FADING_OUT,
        MUSIC_PAUSED
    };
    MusicState musicState;
    Music backgroundMusic;
    bool musicLoaded;
    float masterVolume;
    float currentVolume;
    float fadeDuration;
    float pausedVolume;

    // --- Game State ---
    enum GameState {
        GAME_PLAYING_STATE,
        GAME_GAME_OVER,
        GAME_COUNTDOWN,
        GAME_PAUSED
    };
    GameState gameState;

    // --- Game Over Sequence ---
    enum GameOverPhase {
        FREEZE,
        FADE_BLUR,
        RED_FLASH,
        TEXT_DISPLAY
    };
    GameOverPhase gameOverPhase;
    float gameOverTimer;
    float blurOpacity;
    float countdownTimer;
    static constexpr float FADE_BLUR_DURATION = 1.0f;
    static constexpr float HOLD_DURATION = 2.0f;
    static constexpr float RED_FLASH_DURATION = 0.5f;
    static constexpr float COUNTDOWN_DURATION = 3.0f;
    static constexpr float MAX_BLUR_OPACITY = 200;
    static constexpr float SLOW_MOTION_FACTOR = 0.25f;

public:
    Game() :
        backgroundRotation(0.0f), rotationSpeed(0.1f), backgroundScale(1.0f),
        musicState(MUSIC_IDLE), musicLoaded(false), masterVolume(0.5f),
        currentVolume(0.0f), fadeDuration(2.5f), pausedVolume(0.0f),
        gameState(GAME_PLAYING_STATE), gameOverPhase(FREEZE), gameOverTimer(0.0f), blurOpacity(0.0f), countdownTimer(0.0f) {

        InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pussy Invaders - Game Over Demo");
        SetWindowState(FLAG_WINDOW_RESIZABLE);
        SetTargetFPS(60);

        InitAudioDevice();
        if (IsAudioDeviceReady()) std::cout << "Audio device initialized successfully\n";
        else std::cout << "Warning: Audio device could not be initialized!\n";

        backgroundCenter = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
        LoadAssets();
    }

    ~Game() {
        UnloadAssets();
        CloseAudioDevice();
        CloseWindow();
    }

    void Run() {
        while (!WindowShouldClose()) {
            Update();
            Draw();
        }
    }

private:
    void LoadAssets() {
        backgroundTexture = LoadTexture("Background-1.jpg");
        if (backgroundTexture.id == 0) {
            std::cout << "Warning: Could not load background texture. Using fallback starfield.\n";
            backgroundTexture = { 0, SCREEN_WIDTH, SCREEN_HEIGHT, 1, 1 };
        }
        CalculateBackgroundScale();

        LoadBackgroundMusic();
        if (musicLoaded) {
            PlayMusicStream(backgroundMusic);
            SetMusicVolume(backgroundMusic, 0.0f);
            musicState = MUSIC_FADING_IN;
        }
    }

    void UnloadAssets() {
        if (musicLoaded) UnloadMusicStream(backgroundMusic);
        if (backgroundTexture.id != 0) UnloadTexture(backgroundTexture);
    }

    void LoadBackgroundMusic() {
        const std::vector<const char*> musicFiles = {
            "OIIAOIIA CAT x AFTER DARK.mp3",
            "background_music.mp3", "background_music.ogg", "background_music.wav",
            "space_music.mp3", "space_music.ogg", "music.mp3", "music.ogg"
        };
        for (const auto& file : musicFiles) {
            if (FileExists(file)) {
                backgroundMusic = LoadMusicStream(file);
                if (backgroundMusic.stream.buffer != nullptr) {
                    musicLoaded = true;
                    std::cout << "Successfully loaded music: " << file << std::endl;
                    return;
                }
            }
        }
        std::cout << "Warning: Could not load any background music files.\n";
    }

    void Update() {
        if (gameState == GAME_PLAYING_STATE) {
            backgroundRotation += rotationSpeed * GetFrameTime();
            HandleMusic();
            HandleInput();
        }
        else if (gameState == GAME_GAME_OVER) {
            if (gameOverPhase == FREEZE || gameOverPhase == FADE_BLUR) {
                backgroundRotation += rotationSpeed * GetFrameTime() * SLOW_MOTION_FACTOR;
            }
            HandleGameOver();
            if (gameOverPhase == TEXT_DISPLAY && IsKeyPressed(KEY_R)) HandleInput();
        }
        else if (gameState == GAME_COUNTDOWN) {
            float speedFactor = SLOW_MOTION_FACTOR + (1.0f - SLOW_MOTION_FACTOR) * (countdownTimer / COUNTDOWN_DURATION);
            backgroundRotation += rotationSpeed * GetFrameTime() * speedFactor;
            HandleCountdown();
            HandleMusic();
        }
        else if (gameState == GAME_PAUSED) {
            if (musicLoaded) {
                UpdateMusicStream(backgroundMusic);
            }
            HandleInput();
        }
        if (IsWindowResized()) {
            backgroundCenter = { (float)GetScreenWidth() / 2.0f, (float)GetScreenHeight() / 2.0f };
            CalculateBackgroundScale();
        }
    }

    void HandleMusic() {
        if (!musicLoaded || gameState == GAME_GAME_OVER) return;

        UpdateMusicStream(backgroundMusic);

        switch (musicState) {
        case MUSIC_FADING_IN: {
            currentVolume += (pausedVolume > 0.0f ? pausedVolume : masterVolume) / fadeDuration * GetFrameTime();
            if (currentVolume >= (pausedVolume > 0.0f ? pausedVolume : masterVolume)) {
                currentVolume = pausedVolume > 0.0f ? pausedVolume : masterVolume;
                musicState = MUSIC_PLAYING;
            }
            SetMusicVolume(backgroundMusic, currentVolume);
            break;
        }
        case MUSIC_PLAYING: {
            float musicLength = GetMusicTimeLength(backgroundMusic);
            float timePlayed = GetMusicTimePlayed(backgroundMusic);
            if (musicLength > 0 && (musicLength - timePlayed) <= fadeDuration) {
                musicState = MUSIC_FADING_OUT;
            }
            break;
        }
        case MUSIC_FADING_OUT: {
            currentVolume -= (masterVolume / fadeDuration) * GetFrameTime();
            if (currentVolume <= 0.0f) {
                currentVolume = 0.0f;
                musicState = MUSIC_IDLE;
                StopMusicStream(backgroundMusic);
                PlayMusicStream(backgroundMusic);
                musicState = MUSIC_FADING_IN;
                SetMusicVolume(backgroundMusic, currentVolume);
            }
            else {
                SetMusicVolume(backgroundMusic, currentVolume);
            }
            break;
        }
        case MUSIC_IDLE:
        case MUSIC_PAUSED:
            break;
        }
    }

    void HandleInput() {
        if (gameState == GAME_PLAYING_STATE) {
            if (IsKeyPressed(KEY_M) && musicLoaded) {
                if (musicState == MUSIC_PLAYING || musicState == MUSIC_FADING_IN) {
                    musicState = MUSIC_FADING_OUT;
                }
                else if (musicState == MUSIC_IDLE) {
                    musicState = MUSIC_FADING_IN;
                    PlayMusicStream(backgroundMusic);
                }
            }
            if (IsKeyPressed(KEY_EQUAL) || IsKeyPressed(KEY_KP_ADD)) {
                masterVolume = std::min(1.0f, masterVolume + 0.1f);
                SetVolumeNow(masterVolume);
            }
            if (IsKeyPressed(KEY_MINUS) || IsKeyPressed(KEY_KP_SUBTRACT)) {
                masterVolume = std::max(0.0f, masterVolume - 0.1f);
                SetVolumeNow(masterVolume);
            }
            if (IsKeyPressed(KEY_G)) {
                gameState = GAME_GAME_OVER;
                gameOverPhase = FREEZE;
                gameOverTimer = 0.0f;
                blurOpacity = 0.0f;
                if (musicLoaded) {
                    StopMusicStream(backgroundMusic);
                    musicState = MUSIC_IDLE;
                    currentVolume = 0.0f;
                    SetMusicVolume(backgroundMusic, currentVolume);
                    std::cout << "Music stopped\n";
                }
                std::cout << "Game Over triggered\n";
            }
            if (IsKeyPressed(KEY_P)) {
                gameState = GAME_PAUSED;
                if (musicLoaded) {
                    pausedVolume = currentVolume;
                    PauseMusicStream(backgroundMusic);
                    musicState = MUSIC_PAUSED;
                    currentVolume = 0.0f;
                    SetMusicVolume(backgroundMusic, currentVolume);
                    std::cout << "Music paused\n";
                }
                std::cout << "Game paused\n";
            }
            if (IsKeyPressed(KEY_F)) {
                ToggleFullscreen();
                backgroundCenter = { (float)GetScreenWidth() / 2.0f, (float)GetScreenHeight() / 2.0f };
                CalculateBackgroundScale();
                std::cout << "Toggled fullscreen\n";
            }
        }
        else if (gameState == GAME_PAUSED) {
            if (IsKeyPressed(KEY_P)) {
                gameState = GAME_COUNTDOWN;
                countdownTimer = 0.0f;
                if (musicLoaded) {
                    if (!IsMusicStreamPlaying(backgroundMusic)) {
                        PlayMusicStream(backgroundMusic);
                        UpdateMusicStream(backgroundMusic);
                    }
                    ResumeMusicStream(backgroundMusic);
                    currentVolume = 0.0f;
                    SetMusicVolume(backgroundMusic, currentVolume);
                    musicState = MUSIC_FADING_IN;
                    std::cout << "Music resumed with fade-in\n";
                }
                std::cout << "Countdown started\n";
            }
        }
        else if (gameState == GAME_GAME_OVER && gameOverPhase == TEXT_DISPLAY) {
            if (IsKeyPressed(KEY_R)) {
                gameState = GAME_COUNTDOWN;
                countdownTimer = 0.0f;
                if (musicLoaded) {
                    PlayMusicStream(backgroundMusic);
                    musicState = MUSIC_FADING_IN;
                    currentVolume = 0.0f;
                    SetMusicVolume(backgroundMusic, currentVolume);
                    std::cout << "Music resumed\n";
                }
                std::cout << "Countdown started\n";
            }
        }
    }

    void HandleGameOver() {
        gameOverTimer += GetFrameTime();
        switch (gameOverPhase) {
        case FREEZE:
            gameOverTimer = 0.0f;
            gameOverPhase = FADE_BLUR;
            break;
        case FADE_BLUR:
            blurOpacity = (gameOverTimer / FADE_BLUR_DURATION) * MAX_BLUR_OPACITY;
            if (gameOverTimer >= FADE_BLUR_DURATION + HOLD_DURATION) {
                gameOverPhase = RED_FLASH;
                gameOverTimer = 0.0f;
                blurOpacity = MAX_BLUR_OPACITY;
            }
            break;
        case RED_FLASH:
            if (gameOverTimer >= RED_FLASH_DURATION) {
                gameOverPhase = TEXT_DISPLAY;
                gameOverTimer = 0.0f;
            }
            break;
        case TEXT_DISPLAY:
            break;
        }
    }

    void HandleCountdown() {
        countdownTimer += GetFrameTime();
        if (countdownTimer >= COUNTDOWN_DURATION) {
            gameState = GAME_PLAYING_STATE;
            gameOverPhase = FREEZE;
            gameOverTimer = 0.0f;
            blurOpacity = 0.0f;
            std::cout << "Game resumed\n";
        }
    }

    void SetVolumeNow(float volume) {
        masterVolume = volume;
        currentVolume = masterVolume;
        SetMusicVolume(backgroundMusic, currentVolume);
        musicState = MUSIC_PLAYING;
        std::cout << "Volume set to: " << (int)(masterVolume * 100) << "%\n";
    }

    void Draw() {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawRotatingBackground();
        if (gameState == GAME_GAME_OVER) {
            DrawGameOver();
        }
        else if (gameState == GAME_COUNTDOWN) {
            DrawCountdown();
        }
        else if (gameState == GAME_PAUSED) {
            DrawPaused();
        }
        else {
            DrawUI();
        }
        EndDrawing();
    }

    void DrawRotatingBackground() {
        if (backgroundTexture.id == 0) {
            for (int i = 0; i < 100; i++) {
                DrawPixel(GetRandomValue(0, GetScreenWidth()), GetRandomValue(0, GetScreenHeight()), WHITE);
            }
            return;
        }
        Rectangle source = { 0, 0, (float)backgroundTexture.width, (float)backgroundTexture.height };
        float scaledWidth = backgroundTexture.width * backgroundScale;
        float scaledHeight = backgroundTexture.height * backgroundScale;
        Rectangle dest = { backgroundCenter.x, backgroundCenter.y, scaledWidth, scaledHeight };
        Vector2 origin = { scaledWidth * 0.5f, scaledHeight * 0.5f };
        DrawTexturePro(backgroundTexture, source, dest, origin, backgroundRotation, WHITE);
    }

    void DrawGameOver() {
        DrawTexturePro(backgroundTexture,
            { 0, 0, (float)backgroundTexture.width, (float)backgroundTexture.height },
            { backgroundCenter.x, backgroundCenter.y, backgroundTexture.width * backgroundScale, backgroundTexture.height * backgroundScale },
            { (backgroundTexture.width * backgroundScale) * 0.5f, (backgroundTexture.height * backgroundScale) * 0.5f },
            backgroundRotation, { 255, 255, 255, 255 });
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), { 0, 0, 0, (unsigned char)std::min(blurOpacity, (float)MAX_BLUR_OPACITY) });

        switch (gameOverPhase) {
        case FREEZE:
        case FADE_BLUR:
            break;
        case RED_FLASH:
            DrawRectangleLinesEx({ 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() }, 10, { 255, 0, 0, 200 });
            break;
        case TEXT_DISPLAY:
            const char* gameOverText = "GAME OVER";
            int fontSize = 80;
            int textWidth = MeasureText(gameOverText, fontSize);
            DrawText(gameOverText, (GetScreenWidth() - textWidth) / 2, GetScreenHeight() / 2 - fontSize / 2, fontSize, RED);
            const char* restartText = "Press R to Restart";
            int restartFontSize = 30;
            int restartTextWidth = MeasureText(restartText, restartFontSize);
            DrawText(restartText, (GetScreenWidth() - restartTextWidth) / 2, GetScreenHeight() / 2 + fontSize, restartFontSize, WHITE);
            break;
        }
    }

    void DrawCountdown() {
        DrawRotatingBackground();
        float fadeOutOpacity = MAX_BLUR_OPACITY * (1.0f - countdownTimer / COUNTDOWN_DURATION);
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), { 0, 0, 0, (unsigned char)fadeOutOpacity });
        int count = 3 - static_cast<int>(countdownTimer);
        if (count >= 1) {
            const char* countText = TextFormat("%d", count);
            int fontSize = 100;
            int textWidth = MeasureText(countText, fontSize);
            DrawText(countText, (GetScreenWidth() - textWidth) / 2, GetScreenHeight() / 2 - fontSize / 2, fontSize, WHITE);
        }
    }

    void DrawPaused() {
        DrawRotatingBackground();
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), { 0, 0, 0, 150 });
        const char* pauseText = "PAUSED";
        int fontSize = 80;
        int textWidth = MeasureText(pauseText, fontSize);
        DrawText(pauseText, (GetScreenWidth() - textWidth) / 2, GetScreenHeight() / 2 - fontSize / 2, fontSize, WHITE);
        const char* resumeText = "Press P to Resume";
        int resumeFontSize = 30;
        int resumeTextWidth = MeasureText(resumeText, resumeFontSize);
        DrawText(resumeText, (GetScreenWidth() - resumeTextWidth) / 2, GetScreenHeight() / 2 + fontSize, resumeFontSize, WHITE);
    }

    void DrawUI() {
        DrawText("Pussy Invaders", 50, 50, 40, WHITE);
        DrawFPS(GetScreenWidth() - 100, 10);
        DrawText("Music Controls:", 10, 100, 20, WHITE);
        DrawText("M - Pause/Resume Music", 10, 125, 16, RAYWHITE);
        DrawText("+/- : Volume Up/Down", 10, 145, 16, RAYWHITE);
        DrawText("G - Trigger Game Over (Test)", 10, 165, 16, RAYWHITE);
        DrawText("P - Pause Game", 10, 185, 16, RAYWHITE);
        DrawText("F - Toggle Fullscreen", 10, 205, 16, RAYWHITE);
        if (musicLoaded) {
            const char* statusText = "Idle";
            switch (musicState) {
            case MUSIC_PLAYING: statusText = "Playing"; break;
            case MUSIC_FADING_IN: statusText = "Fading In"; break;
            case MUSIC_FADING_OUT: statusText = "Fading Out"; break;
            case MUSIC_IDLE: statusText = (gameState == GAME_GAME_OVER) ? "Stopped" : "Idle"; break;
            case MUSIC_PAUSED: statusText = "Paused"; break;
            }
            DrawText(TextFormat("Music: %s (Vol: %.0f%%)", statusText, currentVolume * 100), 10, 230, 16, LIME);
        }
        else {
            DrawText("Music: Not Loaded", 10, 230, 16, RED);
        }
    }

    void CalculateBackgroundScale() {
        if (backgroundTexture.id == 0) return;
        float scaleX = (float)GetScreenWidth() / backgroundTexture.width;
        float scaleY = (float)GetScreenHeight() / backgroundTexture.height;
        float baseScale = std::max(scaleX, scaleY);
        float diagonal = sqrtf(GetScreenWidth() * GetScreenWidth() + GetScreenHeight() * GetScreenHeight());
        float imageMinDim = std::min((float)backgroundTexture.width, (float)backgroundTexture.height);
        backgroundScale = diagonal / imageMinDim;
    }
};

int main() {
    Game game;
    game.Run();
    return 0;
}