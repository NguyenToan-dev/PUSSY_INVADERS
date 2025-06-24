#include <raylib.h>
#include <vector>
#include "SpaceShip.h"
#include "bullet.h"

int main()
{
    const int width = 1800;
    const int height = 900;

    InitWindow(width, height, "Space Ship");
    InitAudioDevice(); // <-- Add this line
    SetTargetFPS(30);
    SpaceShip obj;

    // Declare bullets as a vector to store Bullet objects
    std::vector<Bullet> bullets;

    // Load the bullet texture
    Texture2D bulletTexture = LoadTexture("image/bullet.png");

    // Load the shooting sound
    Sound shootSound = LoadSound("sound/lasergun.wav");
    SetSoundVolume(shootSound, 1.0f);

    while (!WindowShouldClose())
    {
        // Update bullets
        for (auto& bullet : bullets) {
            if (bullet.active) bullet.Update();
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw bullets
        for (auto& bullet : bullets) {
            if (bullet.active) bullet.Draw();
        }

        obj.Moving();
        obj.StatusBar();

        EndDrawing();

        if (IsKeyPressed(KEY_SPACE)) {
            obj.Shooting(bullets, &bulletTexture);
            PlaySound(shootSound);
        }
    }

    // Unload resources
    UnloadTexture(bulletTexture);
    UnloadSound(shootSound);

    CloseAudioDevice(); // <-- Add this line
    CloseWindow();
    return 0;
}