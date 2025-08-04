#include<raylib.h>
#include "GameController.h"

int main()
{
    const int width = 1920;
    const int height = 1080;

    InitWindow(width, height, "PUSSY INVADERs");
    SetTargetFPS(144);
    InitAudioDevice();

    GameController controller;

    while(!WindowShouldClose())
    {
        ClearBackground(BLACK);
        BeginDrawing();
            controller.Update();
            controller.Draw();
        EndDrawing();

    }

    CloseAudioDevice();
    CloseWindow();
    return 0;
}

