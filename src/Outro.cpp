
// Outro.cpp

#include "Outro.h"
#include <cmath>
#include <stdlib.h>
#include <string>

// --- Typing audio helpers (mirrors IntroTypingAudio logic) ---
static void GenerateKeyClickSound(short* buffer, int samples, float baseFrequency, int sampleRate)
{
    float randomPitch = (rand() % 20 - 10) / 100.0f;
    float frequency = baseFrequency * (1.0f + randomPitch);

    for (int i = 0; i < samples; i++)
    {
        float t = (float)i / sampleRate;
        float value = (fmod(t * frequency, 1.0f) < 0.5) ? 1.0f : -1.0f;
        float decay = 1.0f - (float)i / samples;
        buffer[i] = (short)(value * 32000 * decay);
    }
}

static Sound generateSimpleSound(int sampleRate, int samples)
{
    std::vector<short> buffer(samples);
    GenerateKeyClickSound(buffer.data(), samples, 100.0f, sampleRate);
    Wave wave;
    wave.frameCount = samples;
    wave.sampleRate = sampleRate;
    wave.sampleSize = 16;
    wave.channels = 1;
    wave.data = buffer.data();
    Sound keySound = LoadSoundFromWave(wave);
    return keySound;
}

// --- Outro implementation ---

Outro::Outro()
{
    LoadScenes();
    if (!IsAudioDeviceReady()) {
        TraceLog(LOG_ERROR, "Audio device failed to initialize (Outro)!");
    }

    // background music (optional) - change path to your file
    music = LoadMusicStream("sound/Once Upon a Time.mp3");
    PlayMusicStream(music);
    SetMusicVolume(music, 0.5f);   
    // typing sound used for text
    typingSound = generateSimpleSound(44100, 4410);
    SetSoundVolume(typingSound, 0.25f); 

    // comm/deep voice sound to play when the enemy speaks
    commSound = LoadSound("sound/enemysound.mp3");
    SetSoundVolume(commSound, 1.2f);

    // pilot voice sound
    pilotSound = LoadSound("sound/pilotsound.mp3");
    SetSoundVolume(pilotSound, 1.1f);

    // font - match your project font
    font = LoadFontEx("font/PixelOperatorMonoHB8.ttf", 32, 0, 0);

    allowSkipButton = true;
    sceneDuration = 16.0f; // a bit shorter per scene for outro
    displayedChars = 0;

    // đảm bảo trạng thái ban đầu hợp lệ
    endingTriggered = false;
    endingTextTimer = 0.0f;
    showEndingText = false;

    waiting = false;
    waitTimer = 0.0f;

    commPlayed = false;
    sceneTimer = 0.0f;
    displayedChars = 0;

}

Outro::~Outro()
{
    for (auto& img : images) UnloadTexture(img);
    UnloadMusicStream(music);
    UnloadSound(commSound);
    UnloadSound(typingSound);
    UnloadSound(pilotSound);
    UnloadFont(font);
}

void Outro::LoadScenes()
{
    scenes = {
        { "image/outro1.jpeg", "Earth is safe... or is it only temporary?" },
        { "image/outro2.jpeg", "After Mechacat Supreme explodes in a blinding flash of light, the night sky returns to peace. Burning fragments fall into the ocean like fireworks of victory." },
        { "image/outro3.png", "You leave the atmosphere, carrying the belief that the war is finally over... until an emergency signal echoes from the Lunar Observation Station:" },
        { "image/outro4.png", "\"Pilot! We've detected a massive space gate opening in Mars' orbit... Origin: The Sirius System!\"" },
        { "image/outro5.png", "From the rift of the cosmos, armored warships shaped like giant dogs emerge. Their eyes glow fiercely, steel jaws grinding with menace..." },
        { "image/outro6.png", "A deep, commanding voice resonates through the comm channel:\n\n\"We are the Puppy Defenders... and this is our hunt!\"" }
    };

    for (auto& scene : scenes) {
        // Load ảnh gốc
        Image img = LoadImage(scene.imagePath.c_str());
        // Resize (giữ tỉ lệ phù hợp với màn hình, ở đây ép về 1280x1000)
        ImageResize(&img, 1280, 1000);
        // Convert sang texture
        Texture2D tex = LoadTextureFromImage(img);
        images.push_back(tex);
        // Giải phóng image RAM
        UnloadImage(img);
    }
}


void Outro::Update()
{
    if (finished) return;

    UpdateMusicStream(music);
    sceneTimer += GetFrameTime();

    float typingSpeed = 0.09f; // lower = faster typing
    int totalChars = (int)scenes[currentScene].text.length();
    int currentCharsToShow = (int)(sceneTimer / typingSpeed);
    if (currentCharsToShow > totalChars) currentCharsToShow = totalChars;

    if (displayedChars < currentCharsToShow) {
        displayedChars++;

        // chỉ play typing khi không ở scene voice
        bool isPilotScene = (currentScene == 3);  // "Pilot! We've detected..."
        bool isEnemyScene = (currentScene == 5);  // "We are the Puppy Defenders..."

        if (!isPilotScene && !isEnemyScene) {
            if (!IsSoundPlaying(typingSound)) {
                PlaySound(typingSound);
            }
        }
    }


    // Play comm voice in the scene where the speech exists (scene 5 in zero-based)
    if (currentScene == (int)scenes.size() - 1) { // scene 5
        size_t quotePos = scenes[currentScene].text.find("We are the Puppy Defenders");
        if (quotePos != std::string::npos && displayedChars >= (int)quotePos && !commPlayed) {
            PauseMusicStream(music);      // dừng nhạc
            StopSound(typingSound);       // tắt typing
            PlaySound(commSound);         // phát voice
            commPlayed = true;
        }
    }
    else {
        commPlayed = false;
    }


    if (currentScene == 3) {
        size_t quotePos = scenes[currentScene].text.find("Pilot!");
        if (quotePos != std::string::npos && displayedChars >= (int)quotePos && !pilotPlayed) {
            PauseMusicStream(music);      // dừng nhạc
            StopSound(typingSound);       // tắt typing
            PlaySound(pilotSound);        // phát voice
            pilotPlayed = true;
        }
    }
    else {
        pilotPlayed = false;
    }

    if (!IsSoundPlaying(commSound) && !IsSoundPlaying(pilotSound)) {
        ResumeMusicStream(music);
    }


    // Handle skip button
    bool skipPressed = allowSkipButton &&
        (IsKeyPressed(KEY_ENTER) ||
            (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
                CheckCollisionPointRec(GetMousePosition(), { (float)(GetScreenWidth() - 140), (float)(GetScreenHeight() - 60), 120.0f, 40.0f })));

    bool isLastScene = currentScene == (int)scenes.size() - 1;

    if (skipPressed) {
        if (!isLastScene) {
            AdvanceScene();
        }
        else {
            // Nếu bấm Skip ở cảnh cuối, bật ending và cho text hiện luôn
            endingTriggered = true;
            endingTextTimer = 1.1f;   // vượt ngưỡng > 1.0f => showEndingText = true
            showEndingText = true;
        }

    }
    else if (!isLastScene && sceneTimer > sceneDuration) {
        AdvanceScene();
    }
    else if (isLastScene && sceneTimer > sceneDuration && !endingTriggered) {
        endingTriggered = true;
        endingTextTimer = 0.0f; // start ending timeline
    }

    if (endingTriggered) {
        endingTextTimer += GetFrameTime();

        // after a short fade, show the "To be continued" text
        if (endingTextTimer > 1.0f && !showEndingText) {
            showEndingText = true;
        }

        // when enough time passes after showing end text, finish the outro
        if (showEndingText && endingTextTimer > 6.0f) {
            finished = true;
        }
    }
}

void Outro::Draw()
{
    if (finished) return;

    ClearBackground(BLACK);

    Texture2D tex = images[currentScene];
    float scale = 0.6f;
    float imgWidth = tex.width * scale;
    int imgHeight = tex.height * scale;
    float posX = GetScreenWidth() / 2 - imgWidth / 2;
    int posY = 60;

    DrawTextureEx(tex, { (float)posX, (float)posY }, 0.0f, scale, WHITE);

    // Draw text up to displayed characters
    std::string fullText = scenes[currentScene].text;
    std::string textToShow = fullText.substr(0, displayedChars);

    float textPosX = (float)posX;
    float textPosY = posY + imgHeight + 20;
    float maxTextWidth = imgWidth;

    DrawMultilineText(textToShow, { textPosX, textPosY }, maxTextWidth, 28, 1, WHITE);

    // Draw skip button
    if (allowSkipButton) {
        Rectangle skipButton = { GetScreenWidth() - 140, GetScreenHeight() - 60, 120, 40 };
        DrawRectangleRec(skipButton, DARKGRAY);
        Vector2 skipTextSize = MeasureTextEx(font, "Skip", 24, 1);
        float skipTextX = skipButton.x + (skipButton.width - skipTextSize.x) / 2;
        float skipTextY = skipButton.y + (skipButton.height - skipTextSize.y) / 2;
        DrawTextEx(font, "Skip", { skipTextX, skipTextY }, 24, 1, WHITE);
    }

    // If ending stage reached and showEndingText, fade to black and draw the final line
    if (endingTriggered) {
        float a = fminf(fmaxf((endingTextTimer - 0.2f) / 1.0f, 0.0f), 1.0f);
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, a));

        if (showEndingText) {
            const char* ending = "To be continued... in Puppy Defenders: War for the Earth";
            Vector2 textSize = MeasureTextEx(font, ending, 28, 1);
            float x = GetScreenWidth() / 2 - textSize.x / 2;
            float y = GetScreenHeight() - textSize.y - 80; // cách đáy 80px
            DrawTextEx(font, ending, { x, y }, 28, 1, WHITE);
        }
    }

}

void Outro::DrawMultilineText(const std::string& text, Vector2 pos, float maxWidth, float fontSize, float spacing, Color color)
{
    std::vector<std::string> lines;
    std::string currentLine;
    std::string currentWord;

    for (size_t i = 0; i < text.length(); ++i) {
        char c = text[i];
        if (c == '\n') {
            if (!currentWord.empty()) {
                std::string test = currentLine.empty() ? currentWord : currentLine + ' ' + currentWord;
                Vector2 size = MeasureTextEx(font, test.c_str(), fontSize, spacing);
                if (size.x > maxWidth && !currentLine.empty()) {
                    lines.push_back(currentLine);
                    currentLine = currentWord;
                }
                else {
                    currentLine = test;
                }
                currentWord.clear();
            }
            if (!currentLine.empty()) {
                lines.push_back(currentLine);
                currentLine.clear();
            }
        }
        else if (c == ' ') {
            if (!currentWord.empty()) {
                std::string test = currentLine.empty() ? currentWord : currentLine + ' ' + currentWord;
                Vector2 size = MeasureTextEx(font, test.c_str(), fontSize, spacing);
                if (size.x > maxWidth && !currentLine.empty()) {
                    lines.push_back(currentLine);
                    currentLine = currentWord;
                }
                else {
                    currentLine = test;
                }
                currentWord.clear();
            }
        }
        else {
            currentWord += c;
        }
    }

    if (!currentWord.empty()) {
        std::string test = currentLine.empty() ? currentWord : currentLine + ' ' + currentWord;
        Vector2 size = MeasureTextEx(font, test.c_str(), fontSize, spacing);
        if (size.x > maxWidth && !currentLine.empty()) {
            lines.push_back(currentLine);
            currentLine = currentWord;
        }
        else {
            currentLine = test;
        }
    }
    if (!currentLine.empty()) {
        lines.push_back(currentLine);
    }

    for (size_t i = 0; i < lines.size(); ++i) {
        DrawTextEx(font, lines[i].c_str(),
            { pos.x, pos.y + i * (fontSize + spacing) },
            fontSize, spacing, color);
    }
}

bool Outro::IsFinished() const {
    return finished;
}

void Outro::AdvanceScene()
{
    sceneTimer = 0.0f;
    displayedChars = 0;
    if (currentScene < (int)scenes.size() - 1) {
        currentScene++;
        waiting = false;
        commPlayed = false;
    }
    else {
        if (!waiting) {
            waiting = true;
            waitTimer = 0.0f;
        }
    }
}
