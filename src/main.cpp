#include <raylib.h>
#include "GameController.h"
#include "Intro.h"  // Thêm dòng này để dùng lớp Intro

int main()
{
    const int width = 1920;
    const int height = 1080;

    InitWindow(width, height, "PUSSY INVADERs");
    SetTargetFPS(30);
    InitAudioDevice();

    // ===== Chạy INTRO trước =====
    Intro intro;

    while (!intro.IsFinished() && !WindowShouldClose()) {
        UpdateMusicStream(intro.getMusic()); // Nếu bạn có nhạc
        intro.Update();

        BeginDrawing();
        ClearBackground(BLACK);
        intro.Draw();
        EndDrawing();
    }

    // ===== Sau khi Intro kết thúc, chạy GAME =====
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
