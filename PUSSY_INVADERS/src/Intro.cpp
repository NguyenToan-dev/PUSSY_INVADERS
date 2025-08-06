#include "Intro.h"
#include "IntroTypingAudio.h"

Intro::Intro()
{
    LoadScenes();
    if (!IsAudioDeviceReady()) {
        TraceLog(LOG_ERROR, "Audio device failed to initialize!");
    }
    music = LoadMusicStream("sound/Once Upon a Time.mp3");
    typingSound = generateSimpleSound(44100, 4410); // sampleRate = 44100, duration = 4410 samples (~0.1s)
    SetSoundVolume(typingSound, 0.2f);  // Âm lượng từ 0.0 (im lặng) đến 1.0 (tối đa)
    PlayMusicStream(music);

    meowSound = LoadSound("sound/meow.mp3");

    font = LoadFontEx("font/PixelOperatorMonoHB8.ttf", 32, 0, 0);
    allowSkipButton = true;
    sceneDuration = 20.0f;
    displayedChars = 0; // Initialize displayed characters
}

Intro::~Intro() {
    for (auto& img : images) UnloadTexture(img);
    UnloadMusicStream(music);
    UnloadSound(meowSound);
    UnloadSound(typingSound);
    UnloadFont(font);
}

void Intro::LoadScenes() {
    scenes = {
        { "image/intro1.jpeg", "In 2077, humanity thought their greatest enemy was artificial intelligence.\nBut they were wrong..." },
        { "image/intro2.jpg", "From a strange dimension, \n the Cosmic Black Cat Army pierced through space-time, \n flooding the Milky Way to... TAKE REVENGE!!!!" },
        { "image/intro3.jpg", "The reason? Because humans once... raised them, \n abandoned them, and... neutered them." },
        { "image/intro4.jpg", "You – an anonymous pilot – receive a top-secret mission: \n Fly the combat spaceship, \n repel the 'Pussy Invaders' before Earth becomes... \n A GIANT LITTER BOX!!!!" },
        { "image/intro5.jpg", "\"It's time... to destroy humanity!!!!\"\n\"Meowwwwwwwwwwwwwwww!\"" }
    };

    for (size_t i = 0; i < scenes.size(); ++i) {
        if (i == 2) {
            // Resize riêng ảnh thứ 3 (index = 2)
            Image img = LoadImage(scenes[i].imagePath.c_str());
            ImageResize(&img, 1280, 1000);
            images.push_back(LoadTextureFromImage(img));
            UnloadImage(img);
        }
        else {
            // Các ảnh khác giữ nguyên
            images.push_back(LoadTexture(scenes[i].imagePath.c_str()));
        }
    }
}
void Intro::Update() {
    if (finished) return;

    UpdateMusicStream(music);
    sceneTimer += GetFrameTime();

    if (IsKeyPressed(KEY_F)) {
        ToggleFullscreen();
    }

    float typingSpeed = 0.1f;
    int totalChars = (int)scenes[currentScene].text.length();
    int currentCharsToShow = (int)(sceneTimer / typingSpeed);
    if (currentCharsToShow > totalChars) currentCharsToShow = totalChars;

    // Play sound for each new character, only if the previous sound isn't playing
    if (displayedChars < currentCharsToShow) {
        displayedChars++;

        if (!IsSoundPlaying(typingSound)) {
            PlaySound(typingSound);
        }
    }

    // Play meow sound in the last scene
    if (currentScene == scenes.size() - 1) {
        size_t meowPos = scenes[currentScene].text.find("Meowwwwwwwwwwwwwwww!");
        if (meowPos != std::string::npos && displayedChars >= (int)meowPos && !meowPlayed) {
            PlaySound(meowSound);
            meowPlayed = true;
        }
    }
    else {
        meowPlayed = false; // Reset when not in last scene
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
            finished = true;
        }
    }
    else if (!isLastScene && sceneTimer > sceneDuration) {
        AdvanceScene();
    }
    else if (isLastScene && sceneTimer > sceneDuration && !waiting) {
        waiting = true;
        waitTimer = 0.0f;
    }

    if (waiting) {
        waitTimer += GetFrameTime();
        if (waitTimer > 10.0f || (skipPressed && isLastScene)) {
            finished = true;
        }
    }
}

void Intro::Draw() {
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
}

void Intro::DrawMultilineText(const std::string& text, Vector2 pos, float maxWidth, float fontSize, float spacing, Color color) {
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

bool Intro::IsFinished() const {
    return finished;
}

void Intro::AdvanceScene() {
    sceneTimer = 0.0f;
    displayedChars = 0; // Reset displayed characters for new scene
    if (currentScene < (int)scenes.size() - 1) {
        currentScene++;
        waiting = false;
        meowPlayed = false;
    }
    else {
        if (!waiting) {
            waiting = true;
            waitTimer = 0.0f;
        }
    }
}