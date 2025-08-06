// ================================
// Menu.h
// ================================
#pragma once
#include <raylib.h>
#include "Background.h"

enum MenuState {
    MENU_MAIN,
    MENU_SETTINGS,
    MENU_CREDITS,
    MENU_EXIT_TO_GAME
};

class Menu {
private:
    MenuState currentState;
    int selectedOption;
    Texture2D titleTexture;
    Font font;
    Background menuBackground;


    // Menu options
    const char* mainMenuOptions[3] = { "PLAY", "SETTINGS", "CREDITS" };
    const char* settingsOptions[2] = { "VOLUME", "BACK" };

    // Settings
    float masterVolume;

    // Animation/visual effects
    float pulseTimer;
    float backgroundOffset;

    void UpdateMainMenu();
    void UpdateSettings();
    void UpdateCredits();

    void DrawMainMenu();
    void DrawSettings();
    void DrawCredits();
    void DrawBackground();



public:
    Menu();
    ~Menu();

    void Update();
    void Draw();

    bool ShouldExitToGame() const;
    float GetMasterVolume() const;
};