// SpaceShip.cpp

#include "SpaceShip.h"
#include "Pickup.h"
#include <random>

// Thiết lập giới hạn nhiệt độ
int SpaceShip::heat_limit = 500;

// NOTE: Spaceship scale factor - adjust this to make ship bigger/smaller
// 0.2f = normal size, 0.15f = smaller, 0.25f = larger
float SpaceShip::ship_scale = 0.1f; // Made smaller as requested

// Constructor: khởi tạo các thuộc tính ban đầu
SpaceShip::SpaceShip()
{
    live_counter = 5; // Mạng sống ban đầu
    missile_counter = 0; // Số tên lửa
    weapon_level = 0; // Cấp độ vũ khí
    thigh_counter = 0; // Thigh item
    overheat = 0; // Nhiệt độ hiện tại
    score = 0; // Điểm số

    // Initialize recoil variables
    recoil_offset = { 0.0f, 0.0f };
    recoil_timer = 0.0f;
    recoil_duration = 0.2f; // Recoil lasts for 0.1 seconds
    recoil_strength = 20.0f; // How far back the ship moves

    InsertSpaceShipTexture(); // Load hình ảnh tàu
    font = LoadFont("font/ChonkyBitsFontBold.otf"); // Load font chữ
}

// Đặt lại toàn bộ thuộc tính về mặc định
void SpaceShip::SetAttribute()
{
    live_counter = 5;
    missile_counter = 0;
    weapon_level = 0;
    thigh_counter = 0;
    overheat = 0;
    score = 0;

    // Reset recoil
    recoil_offset = { 0.0f, 0.0f };
    recoil_timer = 0.0f;

    InsertSpaceShipTexture();
    font = LoadFont("font/ChonkyBitsFontBold.otf");
}

// Load texture các thành phần của tàu
void SpaceShip::InsertSpaceShipTexture()
{
    image.SetTexture();
    return;
}

// Destructor: giải phóng tài nguyên
SpaceShip::~SpaceShip()
{
    UnloadFont(font);
}

// Getter – trả về texture của tàu
Texture2D SpaceShip::GetShip() const { return image.ship.texture; }

// Getter – trả về texture của lửa đẩy
Texture2D SpaceShip::GetFireball() const { return image.fireball.texture; }

// NOTE: SetShipScale() - Use this function to change ship size during runtime
void SpaceShip::SetShipScale(float scale) {
    ship_scale = scale;
}

// NOTE: GetShipScale() - Get current ship scale
float SpaceShip::GetShipScale() const {
    return ship_scale;
}

// Update recoil effect
void SpaceShip::UpdateRecoil()
{
    if (recoil_timer > 0.0f) {
        recoil_timer -= GetFrameTime();

        // Calculate recoil progress (1.0 at start, 0.0 at end)
        float progress = recoil_timer / recoil_duration;
        if (progress < 0.0f) progress = 0.0f;

        // Use smooth easing for natural recoil movement
        float eased_progress = progress * progress * (3.0f - 2.0f * progress); // Smoothstep

        // Apply recoil offset (moving ship backward/downward)
        recoil_offset.x = 0.0f; // No horizontal recoil
        recoil_offset.y = recoil_strength * eased_progress;
    }
    else {
        recoil_offset = { 0.0f, 0.0f };
    }
}

// Apply recoil when shooting
void SpaceShip::ApplyRecoil()
{
    recoil_timer = recoil_duration;

    // Add some randomness to recoil direction for variety
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(-1.0f, 1.0f);

    // Small horizontal component for more realistic feel
    float horizontal_variance = dis(gen) * 2.0f;
    recoil_offset.x = horizontal_variance;
}

// Hiển thị tàu theo vị trí chuột
void SpaceShip::Moving()
{
    UpdateRecoil(); // Update recoil effect

    Vector2 mouse = GetMousePosition();
    Vector2 pos = BoundChecking(mouse);

    // Apply recoil offset to ship position
    pos.x += recoil_offset.x;
    pos.y += recoil_offset.y;

    // Draw ship and fireball
    DrawTextureEx(image.ship.texture, pos, 0.0f, ship_scale, WHITE);
    Vector2 pos1 = RocketPosition(pos);
    Color tint = fireball_brightness.TintColor();
    DrawTextureEx(image.fireball.texture, pos1, 0.0f, ship_scale, tint);

    return;
}

// Hiển thị tàu khi bị trúng đạn (nhấp nháy)
void SpaceShip::MovingWhileBlinking(Color shiptint)
{
    UpdateRecoil(); // Update recoil effect

    Vector2 mouse = GetMousePosition();
    Vector2 pos = BoundChecking(mouse);

    // Apply recoil offset to ship position
    pos.x += recoil_offset.x;
    pos.y += recoil_offset.y;

    // Draw ship and fireball
    DrawTextureEx(image.ship.texture, pos, 0.0f, ship_scale, shiptint);
    Vector2 pos1 = RocketPosition(pos);
    DrawTextureEx(image.fireball.texture, pos1, 0.0f, ship_scale, shiptint);

    return;
}

// Hiển thị thanh trạng thái (status bar)
void SpaceShip::StatusBar()
{
    UpdateStatus(HEAT_DECREASE); // làm mát dần

    // Hiển thị thông tin
    DrawTextEx(font, TextFormat("SCORE: %llu", score), { 0.0f, 0.0f }, 40.0f, 0.0f, GREEN);
    DrawTextEx(font, TextFormat("HEAT: %d/%d", overheat, heat_limit), { 0.0f, 40.0f }, 40.0f, 0.0f, GREEN);

    // Move the status bar image slightly upward
    Vector2 adjustedBarPos = image.bar.pos;
    adjustedBarPos.y -= 20.0f;
    DrawTextureEx(image.bar.texture, adjustedBarPos, 0.0f, 1.0f, WHITE);

    // Initialize position vector for item counts
    Vector2 pos = { 0.0f, static_cast<float>(GetScreenHeight() - 50) };

    // Draw item counts with proper positioning
    pos.x = image.live.pos.x + 50;
	pos.y = image.live.pos.y - 5; // Adjust Y position for better alignment
    DrawTextEx(font, TextFormat("\t%d\t", live_counter), pos, 40.0f, 0.0f, GREEN);

    pos.x = image.missile.pos.x + 0;
    pos.y = image.missile.pos.y + 5; // Adjust Y position for better alignment
    DrawTextEx(font, TextFormat("\t%d\t", missile_counter), pos, 40.0f, 0.0f, GREEN);

    pos.x = image.thigh.pos.x + 0;
    pos.y = image.thigh.pos.y + 5; // Adjust Y position for better alignment
    DrawTextEx(font, TextFormat("\t%d\t", thigh_counter), pos, 40.0f, 0.0f, GREEN);

    pos.x = image.level.pos.x + 0;
    pos.y = image.level.pos.y + 5; // Adjust Y position for better alignment
    DrawTextEx(font, TextFormat("\t%d\t", weapon_level), pos, 40.0f, 0.0f, GREEN);
}


    // Cập nhật trạng thái tàu
    void SpaceShip::UpdateStatus(ShipStatus flag)
    {
        if (flag == HEAT_DECREASE)
        {
            // Giảm nhiệt
            overheat -= 5;
            if (overheat <= 0)
                overheat = 0;
        }
        else if (flag == HEAT_INCREASE)
        {
            // Tăng nhiệt
            if (overheat >= heat_limit)
            {
                overheat = heat_limit;
                return;
            }
            overheat += 40;
        }
        else if (flag >= SCORE_GAIN_1 && flag <= SCORE_GAIN_3)
        {
            // Tăng điểm theo loại kẻ địch
            if (flag == SCORE_GAIN_1)
                score += 50;
            else if (flag == SCORE_GAIN_2)
                score += 100;
            else if (flag == SCORE_GAIN_3)
                score += 150;
        }
        else if (flag == LIVE_DECREASE)
        {
            // Mất mạng
            live_counter--;
            if (live_counter <= 0)
                live_counter = 0;
        }
        else if (flag == LEVEL_UP)
        {
            // Tăng cấp vũ khí tối đa 5
            if (weapon_level >= 5)
                weapon_level = 5;
            else
                weapon_level++;
        }
        else if (flag == MISSILE_ADD)
        {
            // Thêm tên lửa, tối đa 10
            if (missile_counter >= 10)
                missile_counter = 10;
            else
                missile_counter++;
        }
        else if (flag == SUSHI_ADD)
        {
            // Nhặt sushi
            sushi_collected++;

            // Kiểm tra nếu đủ để đổi tên lửa
            int totalSushiValue = sushi_collected + milk_collected * 5;
            while (totalSushiValue >= 10) {
                missile_counter++;
                if (missile_counter > 10) missile_counter = 10;

                // Trừ sushi/milk sau khi đổi
                if (sushi_collected >= 10) {
                    sushi_collected -= 10;
                }
                else {
                    int remaining = 10 - sushi_collected;
                    sushi_collected = 0;
                    int milkToUse = (remaining + 4) / 5;
                    milk_collected -= milkToUse;
                    if (milk_collected < 0) milk_collected = 0;
                }

                totalSushiValue = sushi_collected + milk_collected * 5;
            }
        }
        else if (flag == MILK_ADD)
        {
            // Tương tự sushi – cộng milk và đổi tên lửa
            milk_collected++;

            int totalSushiValue = sushi_collected + milk_collected * 5;
            while (totalSushiValue >= 10) {
                missile_counter++;
                if (missile_counter > 10) missile_counter = 10;

                if (sushi_collected >= 10) {
                    sushi_collected -= 10;
                }
                else {
                    int remaining = 10 - sushi_collected;
                    sushi_collected = 0;
                    int milkToUse = (remaining + 4) / 5;
                    milk_collected -= milkToUse;
                    if (milk_collected < 0) milk_collected = 0;
                }

                totalSushiValue = sushi_collected + milk_collected * 5;
            }
        }
        else if (flag == NEW_BULLET)
        {
            // Nhặt gift – chuyển loại đạn
            // TODO: đặt cờ hoặc thay đổi texture
        }
        else if (flag == LEVEL_UP)
        {
            // Tăng cấp từ battery
            battery_collected++;
            if (weapon_level >= 5)
                weapon_level = 5;
            else
                weapon_level++;
        }

        return;
    }

// Wrapper cho UpdateStatus
void SpaceShip::AdjustStatus(ShipStatus flag)
{
    if (flag <= 10 && flag >= 0)
        UpdateStatus(flag);
    return;
}

// Kiểm tra tàu có ra ngoài màn hình không
Vector2 SpaceShip::BoundChecking(Vector2 mouse)
{
    float width = static_cast<float>(image.ship.texture.width) * ship_scale; // Use scalable size
    float height = static_cast<float>(image.ship.texture.height) * ship_scale; // Use scalable size

    Vector2 pos;
    pos.x = mouse.x - width / 2.0f;
    pos.y = mouse.y - height / 2.0f;

    // Giới hạn trong màn hình
    if (pos.x < 0.0f) pos.x = 0.0f;
    if (pos.y < 0.0f) pos.y = 0.0f;
    if (pos.x + width > static_cast<float>(GetScreenWidth()))
        pos.x = static_cast<float>(GetScreenWidth()) - width;
    if (pos.y + height > static_cast<float>(GetScreenHeight()))
        pos.y = static_cast<float>(GetScreenHeight()) - height;

    return pos;
}

// Vị trí rocket fireball nằm dưới thân tàu
Vector2 SpaceShip::RocketPosition(Vector2 topleft)
{
    float Swidth = static_cast<float>(image.ship.texture.width) * ship_scale; // Use scalable size
    float Sheight = static_cast<float>(image.ship.texture.height) * ship_scale; // Use scalable size
    float Fwidth = static_cast<float>(image.fireball.texture.width) * ship_scale; // Use scalable size
    Vector2 pos;
    pos.y = topleft.y + Sheight;
    pos.x = topleft.x + Swidth / 2.0f - Fwidth / 2.0f;
    return pos;
}

// Bắn đạn ra từ tâm tàu
void SpaceShip::Shooting(std::vector<Bullet>& bullets, Texture2D* bulletTexture)
{
    UpdateStatus(HEAT_INCREASE);
    ApplyRecoil(); // Apply recoil effect when shooting

    Vector2 mouse = GetMousePosition();
    Vector2 pos = BoundChecking(mouse);

    // Apply recoil offset to bullet spawn position for consistency
    Vector2 center;
    center.x = pos.x + static_cast<float>(image.ship.texture.width) * ship_scale / 2.0f + recoil_offset.x; // Use scalable size
    center.y = pos.y + static_cast<float>(image.ship.texture.height) * ship_scale / 2.0f + recoil_offset.y; // Use scalable size
    bullets.emplace_back(center, bulletTexture);
}

// Lấy hình chữ nhật bao tàu – dùng để va chạm
Rectangle SpaceShip::getRect()
{
    Vector2 mouse = GetMousePosition();
    Rectangle ret;

    // --- MODIFICATION: Make hitbox smaller ---
    // This makes the hitbox 70% of the visual size. Change 0.7f to whatever you like.
    constexpr float hitbox_scale_modifier = 0.5f;

    // Calculate the smaller hitbox dimensions
    ret.width = image.ship.texture.width * ship_scale * hitbox_scale_modifier;
    ret.height = image.ship.texture.height * ship_scale * hitbox_scale_modifier;

    // Apply recoil offset and center the hitbox correctly
    // BUG FIX: Used ret.width for X and ret.height for Y to correctly center it.
    ret.x = mouse.x - ret.width / 2.0f + recoil_offset.x;
    ret.y = mouse.y - ret.height / 2.0f + recoil_offset.y;

    return ret;
}

// Kiểm tra va chạm giữa đạn địch và tàu
int SpaceShip::HitBoxChecking(vector<Bullet*>& bullets)
{
    for (auto& bullet : bullets)
    {
        if (CheckCollisionRecs(bullet->getRect(), getRect()))
        {
            bullet->SetActive(false);
            Vector2 pos = bullet->GetPosition();
            pos.x = -9999;
            bullet->SetPosition(pos);
            UpdateStatus(LIVE_DECREASE);
            if (live_counter == 0)
                return 2; // tàu nổ
            return 1; // trúng đạn
        }
    }
    return 0; // không trúng
}

// Kiểm tra và ăn pickup
void SpaceShip::EatPickup()
{
    Rectangle shipRect = getRect();
    const auto& allPickups = Pickup::GetAll();

    for (int i = 0; i < (int)allPickups.size(); ++i) {
        Vector2 pickupPos = allPickups[i].GetPosition();
        float size = 40.f;

        Rectangle pickupRect = {
            pickupPos.x - size / 2,
            pickupPos.y - size / 2,
            size,
            size
        };

        if (CheckCollisionRecs(shipRect, pickupRect)) {
            PickupType type = allPickups[i].GetType();
            switch (type) {
            case PickupType::Sushi:
                AdjustStatus(SUSHI_ADD); break;
            case PickupType::Milk:
                AdjustStatus(MILK_ADD); break;
            case PickupType::Battery:
                AdjustStatus(LEVEL_UP); break;
            case PickupType::Gift1:
            case PickupType::Gift2:
            case PickupType::Gift3:
                AdjustStatus(NEW_BULLET); break;
            default:
                break;
            }

            // Xóa pickup đã ăn
            Pickup::RemoveAt(i);
            --i;
        }
    }
}