#include "raylib.h"
#include "bullet.h"
#include <vector>
#include <algorithm>

int main() {
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Bullet Test - Raylib");
    SetTargetFPS(60);

    // Khởi tạo audio
    InitAudioDevice();

    // Load ảnh đạn
    Texture2D bulletTexture = LoadTexture("graphics/bullet.png");
    // Load âm thanh bắn đạn
    Sound shootSound = LoadSound("sound/lasergun.wav");

    std::vector<Bullet> bullets;

    while (!WindowShouldClose()) {
        // Bắn đạn khi nhấn SPACE
        if (IsKeyPressed(KEY_SPACE)) {
            Vector2 startPos = {
                screenWidth / 2.0f,
                screenHeight - 30.0f
            };
            bullets.emplace_back(startPos, &bulletTexture);
            PlaySound(shootSound); // Phát âm thanh bắn đạn
        }

        // Cập nhật đạn
        for (auto& bullet : bullets) {
            bullet.Update();
        }

        // Xóa đạn đã bay ra ngoài
        bullets.erase(
            std::remove_if(bullets.begin(), bullets.end(),
                [](const Bullet& b) { return !b.active; }),
            bullets.end()
        );

        BeginDrawing();
        ClearBackground(BLACK);

        for (auto& bullet : bullets) {
            bullet.Draw();
        }

        DrawText("Press SPACE to shoot", 10, 10, 20, WHITE);

        EndDrawing();
    }

    // Giải phóng tài nguyên
    UnloadTexture(bulletTexture);
    UnloadSound(shootSound);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}