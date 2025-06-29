// main.cpp
#include <raylib.h>
#include <vector>
#include <algorithm>
#include "SpaceShip.h"
#include "bullet.h"
#include "pussy.h"

using namespace std;

int main() {
    const int width = 1800;
    const int height = 900;

    InitWindow(width, height, "Space Ship vs Pussies");
    InitAudioDevice();
    SetTargetFPS(60);

    Pussy::LoadImage();

    SpaceShip ship;
    vector<Bullet> bullets;
    vector<Bullet> pussyBullets;
    vector<Pussy> pussies;

    // Load texture và âm thanh
    Texture2D bulletTexture = LoadTexture("image/bullet.png");
    Texture2D pussyBulletTexture = LoadTexture("image/shit.png");
    Sound shootSound = LoadSound("sound/lasergun.wav");
    Sound pussyShootSound = LoadSound("sound/bom.mp3");

    SetSoundVolume(shootSound, 1.0f);
    SetSoundVolume(pussyShootSound, 1.0f);

    // Khởi tạo pussy
    const int rows = 3;
    const int perRow = 6;
    const float spacingX = 200;
    const float startX = 150;
    const float startY = 100;

    for (int row = 0; row < rows; row++) {
        for (int i = 0; i < perRow; i++) {
            Vector2 pos = { startX + i * spacingX, startY + row * 100 };
            pussies.emplace_back(1, pos);
        }
    }

    int pussyDirection = 2;
    float pussyShootTimer = 0.0f;
    float pussyShootInterval = 1.5f;

    while (!WindowShouldClose()) {
        // Cập nhật đạn tàu
        for (auto& bullet : bullets)
            if (bullet.active) bullet.Update();

        bullets.erase(
            remove_if(bullets.begin(), bullets.end(), [](const Bullet& b) { return !b.active; }),
            bullets.end()
        );

        // Va chạm đạn tàu và pussy
        for (auto& bullet : bullets) {
            if (!bullet.active) continue;
            for (auto& pussy : pussies) {
                if (CheckCollisionRecs(bullet.getRect(), pussy.getRect())) {
                    bullet.active = false;
                    pussy.position.x = -9999;
                    break;
                }
            }
        }

        pussies.erase(
            remove_if(pussies.begin(), pussies.end(), [](const Pussy& p) { return p.position.x < 0; }),
            pussies.end()
        );

        // Pussies di chuyển và bắn
        bool reachEdge = false;
        for (auto& pussy : pussies) {
            pussy.Update(pussyDirection);
            Rectangle r = pussy.getRect();
            if (r.x < 0 || r.x + r.width > width) reachEdge = true;
        }
        if (reachEdge) {
            pussyDirection = -pussyDirection;
            for (auto& pussy : pussies) pussy.position.y += 20;
        }

        pussyShootTimer += GetFrameTime();
        if (pussyShootTimer >= pussyShootInterval && !pussies.empty()) {
            pussyShootTimer = 0.0f;
            int idx = GetRandomValue(0, pussies.size() - 1);
            Pussy& shooter = pussies[idx];
            Vector2 pos = {
                shooter.getRect().x + shooter.getRect().width / 2,
                shooter.getRect().y + shooter.getRect().height
            };
            Bullet pussyBullet(pos, &pussyBulletTexture);
            pussyBullet.speed = -pussyBullet.speed;
            pussyBullets.push_back(pussyBullet);
            PlaySound(pussyShootSound);
        }

        for (auto& b : pussyBullets)
            if (b.active) b.Update();
        pussyBullets.erase(
            remove_if(pussyBullets.begin(), pussyBullets.end(), [](const Bullet& b) { return !b.active; }),
            pussyBullets.end()
        );

        // Tàu di chuyển & bắn
        ship.Moving();
        if (IsKeyPressed(KEY_SPACE)) {
            ship.Shooting(bullets, &bulletTexture);
            PlaySound(shootSound);
        }

        BeginDrawing();
        ClearBackground(BLACK);

        for (auto& b : bullets) if (b.active) b.Draw();
        for (auto& b : pussyBullets) if (b.active) b.Draw();
        for (auto& pussy : pussies) pussy.Draw();
        ship.StatusBar();
        EndDrawing();
    }

    // Giải phóng
    UnloadTexture(bulletTexture);
    UnloadTexture(pussyBulletTexture);
    UnloadSound(shootSound);
    UnloadSound(pussyShootSound);
    Pussy::UnloadImage();
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
