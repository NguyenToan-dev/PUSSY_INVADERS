#include "Intro.h"

Intro::Intro() {
    LoadScenes();
    music = LoadMusicStream("sound/Once Upon a Time.mp3");
    PlayMusicStream(music);

    meowSound = LoadSound("sound/meow.mp3");


    font = LoadFontEx("font/PixelOperatorSC.ttf", 32, 0, 0);
    allowSkipButton = true;
    sceneDuration = 20.0f; 
}

Intro::~Intro() {
    for (auto& img : images) UnloadTexture(img);
    UnloadMusicStream(music);
    UnloadSound(meowSound);
    UnloadFont(font);
}

void Intro::LoadScenes() {
    scenes = {
        { "image/intro1.jpeg", "In 2077, humanity thought their greatest enemy was artificial intelligence.\nBut they were wrong..." },
        { "image/intro2.jpg", "From a strange dimension, \n the Cosmic Black Cat Army pierced through space-time, \n flooding the Milky Way to... TAKE REVENGE!" },
        { "image/intro3.jpg", "The reason? Because humans once... raised them, \n abandoned them, and... neutered them." },
        { "image/intro4.jpg", "You – an anonymous pilot – receive a top-secret mission: \n Fly the combat spaceship, \n repel the 'Pussy Invaders' before Earth becomes... \n A GIANT LITTER BOX!" },
        { "image/intro5.jpg", "\"It's time... to destroy humanity!\"\n\"Meowwwwwwwwwwwwwwwwwwwww!\"" }
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

    // Synchronize meow sound with "Meowwwwwwwwwwwwwwwwwwwww!" text
    if (currentScene == scenes.size() - 1) {
        const std::string& lastText = scenes.back().text;
        size_t meowPos = lastText.find("Meowwwwwwwwwwwwwwwwwwwww!");
        float typingSpeed = 0.1f; // Tăng tốc độ đánh máy 
        int charsToShow = (int)(sceneTimer / typingSpeed);

        // Play meow sound exactly when "Meowwwwwwwwwwwwwwwwwwwww!" starts to appear
        if (meowPos != std::string::npos && charsToShow >= (int)meowPos && !meowPlayed) {
            PlaySound(meowSound);
            meowPlayed = true;
        }
    }
    else {
        meowPlayed = false; // Reset for next time
    }

    bool skipPressed = allowSkipButton &&
        (IsKeyPressed(KEY_ENTER) ||
            (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
                CheckCollisionPointRec(GetMousePosition(), { (float)(GetScreenWidth() - 140), (float)(GetScreenHeight() - 60), 120.0f, 40.0f })));

    bool isLastScene = currentScene == (int)scenes.size() - 1;

    // Chuyển cảnh nếu nhấn Skip hoặc hết thời gian cảnh
    if (skipPressed) {
        if (!isLastScene) {
            AdvanceScene();
        }
        else {
            finished = true;  // Cho phép Skip cảnh cuối ngay lập tức
        }
    }

    else if (!isLastScene && sceneTimer > sceneDuration) {
        AdvanceScene();
    }
    else if (isLastScene && sceneTimer > sceneDuration && !waiting) {
        waiting = true; // Bắt đầu trạng thái chờ
        waitTimer = 0.0f; // Reset bộ đếm chờ
    }

    if (waiting) {
        waitTimer += GetFrameTime(); // Tăng bộ đếm chờ
        if (waitTimer > 10.0f || (skipPressed && isLastScene)) {
            finished = true; // Kết thúc sau 10 giây chờ hoặc nếu nhấn Skip ở cảnh cuối
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

    // Typing effect
    float typingSpeed = 0.1f; // Tăng tốc độ đánh máy để chậm hơn
    int charsToShow = (int)(sceneTimer / typingSpeed);
    std::string fullText = scenes[currentScene].text;
    if (charsToShow > (int)fullText.length()) charsToShow = fullText.length();

    std::string textToShow = fullText.substr(0, charsToShow);

    // Draw text aligned with image
    float textPosX = (float)posX;
    float textPosY = posY + imgHeight + 20;
    float maxTextWidth = imgWidth; // Giới hạn chiều rộng đúng bằng ảnh

    DrawMultilineText(textToShow, { textPosX, textPosY }, maxTextWidth, 28, 1, WHITE);

    // Draw skip button
    if (allowSkipButton) {
        Rectangle skipButton = { GetScreenWidth() - 140, GetScreenHeight() - 60, 120, 40 };
        DrawRectangleRec(skipButton, DARKGRAY);
        DrawTextEx(font, "Skip", { skipButton.x + 30, skipButton.y + 8 }, 24, 1, WHITE);
    }
}


void Intro::DrawMultilineText(const std::string& text, Vector2 pos, float maxWidth, float fontSize, float spacing, Color color) {
    std::vector<std::string> lines;
    std::string currentLine;
    std::string currentWord;

    // Duyệt từng ký tự trong text
    for (size_t i = 0; i < text.length(); ++i) {
        char c = text[i];
        if (c == '\n') {
            // Nếu gặp \n, thêm từ hiện tại vào dòng và kết thúc dòng
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
            // Thêm dòng hiện tại vào danh sách và bắt đầu dòng mới
            if (!currentLine.empty()) {
                lines.push_back(currentLine);
                currentLine.clear();
            }
        }
        else if (c == ' ') {
            // Nếu gặp khoảng trắng, thêm từ hiện tại vào dòng
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
            // Thêm ký tự vào từ hiện tại
            currentWord += c;
        }
    }

    // Xử lý từ cuối cùng và dòng cuối cùng
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

    // Vẽ các dòng
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
    if (currentScene < (int)scenes.size() - 1) {
        currentScene++;
        waiting = false; // Reset trạng thái chờ khi chuyển cảnh
        meowPlayed = false; // Reset meow sound
    }
    else {
        if (!waiting) {
            waiting = true; // Bắt đầu trạng thái chờ
            waitTimer = 0.0f;
        }
    }
}