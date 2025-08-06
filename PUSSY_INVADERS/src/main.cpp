#include <raylib.h>
#include "Menu.h"           // Add this - your new menu system
#include "GameController.h"
#include "Intro.h"

int main()
{
    const int width = 1920;
    const int height = 1080;
    InitWindow(width, height, "PUSSY INVADERs");
    SetTargetFPS(144);
    InitAudioDevice();

    // ===== Run MENU first =====
    Menu menu;
    while (!menu.ShouldExitToGame() && !WindowShouldClose()) {
        menu.Update();
        BeginDrawing();
        ClearBackground(BLACK);
        menu.Draw();
        EndDrawing();
    }

    // Exit if window was closed during menu
    if (WindowShouldClose()) {
        CloseAudioDevice();
        CloseWindow();
        return 0;
    }

    // Set master volume from menu settings
    SetMasterVolume(menu.GetMasterVolume());

    // ===== After menu, run INTRO =====
    Intro intro;
    while (!intro.IsFinished() && !WindowShouldClose()) {
        UpdateMusicStream(intro.getMusic()); // If you have music
        intro.Update();
        BeginDrawing();
        ClearBackground(BLACK);
        intro.Draw();
        EndDrawing();
    }

    // ===== After Intro, run GAME =====
    GameController controller;
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        controller.Update();
        controller.Draw();
        EndDrawing();
    }

    CloseAudioDevice();
    CloseWindow();
    return 0;
}