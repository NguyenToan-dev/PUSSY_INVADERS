// ================================
// Menu.cpp
// ================================
#include "Menu.h"
#include <cmath>
#include <cstring>

Menu::Menu() {
    currentState = MENU_MAIN;
    selectedOption = 0;
    masterVolume = 0.5f;
    pulseTimer = 0.0f;
    backgroundOffset = 0.0f;

    menuBackground.SetRotation(0.5f);

    // Load title texture - with better error handling
    Image titleImage = LoadImage("image/title.png");
    if (titleImage.data != NULL) {
        titleTexture = LoadTextureFromImage(titleImage);
        UnloadImage(titleImage);
    }
    else {
        // Try alternative paths if title.png not found
        titleImage = LoadImage("./title.png");
        if (titleImage.data != NULL) {
            titleTexture = LoadTextureFromImage(titleImage);
            UnloadImage(titleImage);
        }
        else {
            titleImage = LoadImage("assets/title.png");
            if (titleImage.data != NULL) {
                titleTexture = LoadTextureFromImage(titleImage);
                UnloadImage(titleImage);
            }
            else {
                // No texture found - will use text fallback
                titleTexture = { 0 };
            }
        }
    }

    // Load font (use your existing font or default)
    font = LoadFont("font/ChonkyBitsFontBold.otf");
    if (font.texture.id == 0) {
        font = GetFontDefault(); // Fallback to default font
    }
}

Menu::~Menu() {
    UnloadTexture(titleTexture);
    if (font.texture.id != GetFontDefault().texture.id) {
        UnloadFont(font);
    }
}

void Menu::Update() {
    pulseTimer += GetFrameTime();
    backgroundOffset += GetFrameTime() * 20.0f; // Scrolling stars effect

    switch (currentState) {
    case MENU_MAIN:
        UpdateMainMenu();
        break;
    case MENU_SETTINGS:
        UpdateSettings();
        break;
    case MENU_CREDITS:
        UpdateCredits();
        break;
    default:
        break;
    }
}

void Menu::UpdateMainMenu() {
    int maxOptions = 3;

    // Navigation
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
        selectedOption--;
        if (selectedOption < 0) selectedOption = maxOptions - 1;
    }
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
        selectedOption++;
        if (selectedOption >= maxOptions) selectedOption = 0;
    }

    // Selection
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
        switch (selectedOption) {
        case 0: // PLAY
            currentState = MENU_EXIT_TO_GAME;
            break;
        case 1: // SETTINGS
            currentState = MENU_SETTINGS;
            selectedOption = 0;
            break;
        case 2: // CREDITS
            currentState = MENU_CREDITS;
            selectedOption = 0;
            break;
        }
    }
}

void Menu::UpdateSettings() {
    int maxOptions = 2;

    // Navigation
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
        selectedOption--;
        if (selectedOption < 0) selectedOption = maxOptions - 1;
    }
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
        selectedOption++;
        if (selectedOption >= maxOptions) selectedOption = 0;
    }

    // Volume control
    if (selectedOption == 0) {
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
            masterVolume -= GetFrameTime();
            if (masterVolume < 0.0f) masterVolume = 0.0f;
        }
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
            masterVolume += GetFrameTime();
            if (masterVolume > 1.0f) masterVolume = 1.0f;
        }
    }

    // Back to main menu
    if ((IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) && selectedOption == 1) {
        currentState = MENU_MAIN;
        selectedOption = 1; // Return to SETTINGS option
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
        currentState = MENU_MAIN;
        selectedOption = 1;
    }
}

void Menu::UpdateCredits() {
    // Return to main menu
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ESCAPE)) {
        currentState = MENU_MAIN;
        selectedOption = 2; // Return to CREDITS option
    }
}

void Menu::Draw() {
    DrawBackground();

    switch (currentState) {
    case MENU_MAIN:
        DrawMainMenu();
        break;
    case MENU_SETTINGS:
        DrawSettings();
        break;
    case MENU_CREDITS:
        DrawCredits();
        break;
    default:
        break;
    }
}

void Menu::DrawBackground() {
    // Update rotation for animated background
    float currentRotation = menuBackground.GetRotation();
    currentRotation += menuBackground.GetSpeed() * GetFrameTime();
    menuBackground.SetRotation(currentRotation);

    // Draw your game's rotating background
    menuBackground.DrawRotatingBackground();

    // Optional: Add a subtle dark overlay so menu text is more readable
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), ColorAlpha(BLACK, 0.3f));
}

void Menu::DrawMainMenu() {
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    // Draw title
    float titleScale = 0.3f;
    if (titleTexture.id != 0) {
        float titleWidth = titleTexture.width * titleScale;
        float titleHeight = titleTexture.height * titleScale;
        Vector2 titlePos = {
            (screenWidth - titleWidth) / 2.0f,
            screenHeight * 0.15f
        };
        DrawTextureEx(titleTexture, titlePos, 0.0f, titleScale, WHITE);
    }
    else {
        // Fallback text title
        const char* title = "PUSSY INVADERS";
        float titleFontSize = 80.0f;
        Vector2 titleSize = MeasureTextEx(font, title, titleFontSize, 2.0f);
        Vector2 titlePos = {
            (screenWidth - titleSize.x) / 2.0f,
            screenHeight * 0.15f
        };
        DrawTextEx(font, title, titlePos, titleFontSize, 2.0f, WHITE);
    }

    // Draw menu options
    float menuStartY = screenHeight * 0.5f;
    float menuSpacing = 80.0f;
    float fontSize = 50.0f;

    for (int i = 0; i < 3; i++) {
        Vector2 textSize = MeasureTextEx(font, mainMenuOptions[i], fontSize, 2.0f);
        Vector2 pos = {
            (screenWidth - textSize.x) / 2.0f,
            menuStartY + i * menuSpacing
        };

        Color textColor = WHITE;
        if (i == selectedOption) {
            // Pulsing selected option
            float pulse = sin(pulseTimer * 8.0f) * 0.3f + 0.7f;
            textColor = ColorAlpha(GREEN, pulse);

            // Draw selection indicator
            DrawText(">", (int)(pos.x - 60), (int)pos.y, (int)fontSize, GREEN);
            DrawText("<", (int)(pos.x + textSize.x + 20), (int)pos.y, (int)fontSize, GREEN);
        }

        DrawTextEx(font, mainMenuOptions[i], pos, fontSize, 2.0f, textColor);
    }

    // Instructions
    const char* instruction = "USE ARROW KEYS / WASD TO NAVIGATE, ENTER TO SELECT";
    float instrFontSize = 25.0f;
    Vector2 instrSize = MeasureTextEx(font, instruction, instrFontSize, 1.0f);
    Vector2 instrPos = {
        (screenWidth - instrSize.x) / 2.0f,
        screenHeight * 0.9f
    };
    DrawTextEx(font, instruction, instrPos, instrFontSize, 1.0f, GRAY);
}

void Menu::DrawSettings() {
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    // Title
    const char* title = "SETTINGS";
    float titleFontSize = 60.0f;
    Vector2 titleSize = MeasureTextEx(font, title, titleFontSize, 2.0f);
    Vector2 titlePos = {
        (screenWidth - titleSize.x) / 2.0f,
        screenHeight * 0.2f
    };
    DrawTextEx(font, title, titlePos, titleFontSize, 2.0f, WHITE);

    // Volume setting
    float settingsStartY = screenHeight * 0.4f;
    float fontSize = 40.0f;

    // Volume label
    const char* volumeLabel = "MASTER VOLUME";
    Vector2 volumeLabelSize = MeasureTextEx(font, volumeLabel, fontSize, 2.0f);
    Vector2 volumeLabelPos = {
        (screenWidth - volumeLabelSize.x) / 2.0f,
        settingsStartY
    };
    Color volumeColor = (selectedOption == 0) ? GREEN : WHITE;
    DrawTextEx(font, volumeLabel, volumeLabelPos, fontSize, 2.0f, volumeColor);

    // Volume bar
    float barWidth = 300.0f;
    float barHeight = 20.0f;
    Vector2 barPos = {
        (screenWidth - barWidth) / 2.0f,
        settingsStartY + 60.0f
    };

    DrawRectangle((int)barPos.x, (int)barPos.y, (int)barWidth, (int)barHeight, DARKGRAY);
    DrawRectangle((int)barPos.x, (int)barPos.y, (int)(barWidth * masterVolume), (int)barHeight, GREEN);

    // Volume percentage
    const char* volumeText = TextFormat("%.0f%%", masterVolume * 100.0f);
    Vector2 volumeTextSize = MeasureTextEx(font, volumeText, 30.0f, 1.0f);
    Vector2 volumeTextPos = {
        (screenWidth - volumeTextSize.x) / 2.0f,
        barPos.y + 40.0f
    };
    DrawTextEx(font, volumeText, volumeTextPos, 30.0f, 1.0f, WHITE);

    // Back button
    const char* backLabel = "BACK";
    Vector2 backSize = MeasureTextEx(font, backLabel, fontSize, 2.0f);
    Vector2 backPos = {
        (screenWidth - backSize.x) / 2.0f,
        settingsStartY + 150.0f
    };
    Color backColor = (selectedOption == 1) ? GREEN : WHITE;
    DrawTextEx(font, backLabel, backPos, fontSize, 2.0f, backColor);

    if (selectedOption == 1) {
        DrawText(">", (int)(backPos.x - 60), (int)backPos.y, (int)fontSize, GREEN);
        DrawText("<", (int)(backPos.x + backSize.x + 20), (int)backPos.y, (int)fontSize, GREEN);
    }

    // Instructions
    const char* instruction = "LEFT/RIGHT TO ADJUST VOLUME, ENTER TO SELECT, ESC TO GO BACK";
    float instrFontSize = 25.0f;
    Vector2 instrSize = MeasureTextEx(font, instruction, instrFontSize, 1.0f);
    Vector2 instrPos = {
        (screenWidth - instrSize.x) / 2.0f,
        screenHeight * 0.9f
    };
    DrawTextEx(font, instruction, instrPos, instrFontSize, 1.0f, GRAY);
}

void Menu::DrawCredits() {
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    // Title
    const char* title = "CREDITS";
    float titleFontSize = 60.0f;
    Vector2 titleSize = MeasureTextEx(font, title, titleFontSize, 2.0f);
    Vector2 titlePos = {
        (screenWidth - titleSize.x) / 2.0f,
        screenHeight * 0.15f
    };
    DrawTextEx(font, title, titlePos, titleFontSize, 2.0f, WHITE);

    // Star Wars style scrolling credits
    const char* creditsText[] = {
        "PUSSY INVADERS",
        "",
        "DEVELOPED BY:",
        "VO TRUONG HAI",
        "VO DUY LOC",
        "NGUYEN TIEN CUONG",
        "NGUYEN KHANH TOAN",
        "",
        "PROGRAMMING:",
        "VO DUY LOC",
        "NGUYEN KHANH TOAN",
        "NGUYEN TIEN CUONG",
        "VO TRUONG HAI",
        "",
        "ART & DESIGN:",
        "VO TRUONG HAI",
        "",
        "SOUND:",
        "N/A",
        "",
        "SPECIAL THANKS:",
        "Raylib Community",
        "Our Professors - BUI TIEN LEN",
        "",
        "MADE WITH RAYLIB & C++",
        "",
        "",
        "",
        "MAY THE CODE BE WITH YOU..."
    };

    float creditsFontSize = 35.0f;
    float lineSpacing = 50.0f;
    int numLines = sizeof(creditsText) / sizeof(creditsText[0]);

    // Star Wars scrolling effect - credits move upward
    float scrollSpeed = 30.0f; // pixels per second
    float totalScrollTime = pulseTimer * scrollSpeed;
    float creditsStartY = screenHeight + 100.0f - totalScrollTime; // Start from bottom, scroll up

    // Draw credits with perspective effect (far = smaller, near = larger)
    for (int i = 0; i < numLines; i++) {
        if (strlen(creditsText[i]) == 0) continue; // Skip empty lines

        float currentY = creditsStartY + i * lineSpacing;

        // Skip if completely off screen
        if (currentY < -100.0f || currentY > screenHeight + 100.0f) continue;

        // Perspective scaling - closer to center = larger
        float distanceFromCenter = abs(currentY - screenHeight * 0.5f);
        float maxDistance = screenHeight * 0.7f;
        float perspective = 1.0f - (distanceFromCenter / maxDistance);
        if (perspective < 0.3f) perspective = 0.3f; // Minimum scale

        float scaledFontSize = creditsFontSize * perspective;

        // Fade effect based on position
        float alpha = 1.0f;
        if (currentY < screenHeight * 0.2f) {
            alpha = currentY / (screenHeight * 0.2f);
        }
        if (currentY > screenHeight * 0.8f) {
            alpha = (screenHeight - currentY) / (screenHeight * 0.2f);
        }
        if (alpha < 0.0f) alpha = 0.0f;
        if (alpha > 1.0f) alpha = 1.0f;

        Vector2 textSize = MeasureTextEx(font, creditsText[i], scaledFontSize, 1.0f);
        Vector2 pos = {
            (screenWidth - textSize.x) / 2.0f,
            currentY
        };

        Color textColor = ColorAlpha(WHITE, alpha);

        // Changed from GREEN to GREEN for headers
        if (i == 0 || i == 2 || i == 8 || i == 14 || i == 17 || i == 20 || i == 24) {
            textColor = ColorAlpha(GREEN, alpha); // Highlight headers in GREEN
        }

        // Special color for final message
        if (i == numLines - 1) {
            textColor = ColorAlpha(GOLD, alpha);
        }

        DrawTextEx(font, creditsText[i], pos, scaledFontSize, 1.0f, textColor);
    }

    // Reset credits when they finish scrolling
    float totalCreditsHeight = numLines * lineSpacing + screenHeight + 200.0f;
    if (totalScrollTime > totalCreditsHeight) {
        // Reset scroll timer for loop effect
        const_cast<Menu*>(this)->pulseTimer = 0.0f;
    }

    // Instructions
    const char* instruction = "PRESS ANY KEY TO RETURN";
    float instrFontSize = 25.0f;
    Vector2 instrSize = MeasureTextEx(font, instruction, instrFontSize, 1.0f);
    Vector2 instrPos = {
        (screenWidth - instrSize.x) / 2.0f,
        screenHeight * 0.05f // Move to top so it doesn't interfere with credits
    };
    DrawTextEx(font, instruction, instrPos, instrFontSize, 1.0f, GRAY);
}

bool Menu::ShouldExitToGame() const {
    return currentState == MENU_EXIT_TO_GAME;
}

float Menu::GetMasterVolume() const {
    return masterVolume;
}