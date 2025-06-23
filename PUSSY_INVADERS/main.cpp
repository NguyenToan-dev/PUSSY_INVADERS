#include "raylib.h"
#include "bullet.h"
#include <vector>

int main() {
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Bullet Test - Raylib");
    SetTargetFPS(60);

    std::vector<Bullet> bullets;

    while (!WindowShouldClose()) {
        // Bắn đạn khi nhấn SPACE
        if (IsKeyPressed(KEY_SPACE)) {
            Vector2 startPos = { screenWidth / 2.0f, screenHeight - 30.0f };
            bullets.push_back(Bullet(startPos));
        }

        // Cập nhật tất cả viên đạn
        for (auto& bullet : bullets) {
            bullet.Update();
        }

        // Xóa các viên đạn không còn hoạt động
        bullets.erase(
            std::remove_if(bullets.begin(), bullets.end(),
                [](const Bullet& b) { return !b.active; }),
            bullets.end()
        );

        BeginDrawing();
        ClearBackground(BLACK);

        // Vẽ tất cả viên đạn
        for (auto& bullet : bullets) {
            bullet.Draw();
        }

        DrawText("Press SPACE to shoot", 10, 10, 20, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}