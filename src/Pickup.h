#pragma once
#include <raylib.h>
#include <vector>

// Loại vật phẩm (pickup) có thể rơi ra
enum class PickupType {
    Sushi,      // Tăng điểm sushi đổi thành tên lửa
    Battery,    // Nâng cấp cấp độ vũ khí
    Milk,       // Hỗ trợ đổi sushi thành tên lửa
    Gift1,      // Đổi loại đạn
    Gift2,
    Gift3
};

// Lớp đại diện cho một pickup trong game
class Pickup {
public:
    // Trả về danh sách tất cả các pickup hiện có
    static const std::vector<Pickup>& GetAll();

    // Getter vị trí và loại của pickup
    Vector2 GetPosition() const;
    PickupType GetType() const;

    // Xóa pickup tại chỉ số index (dùng khi ăn hoặc hết hạn)
    static void RemoveAt(int index);

    // Constructor: tạo 1 pickup tại vị trí at với loại t
    Pickup(PickupType t, const Vector2& at);

    // Tạo ngẫu nhiên 1 pickup Sushi / Milk / Battery tại vị trí at
    static void Spawn(const Vector2& at);

    // Tạo tự động 1 pickup Gift sau mỗi 15 giây
    static void SpawnGift();

    // Cập nhật toàn bộ pickup mỗi khung hình
    static void UpdateAll(float dt);

    // Vẽ toàn bộ pickup ra màn hình
    static void DrawAll();

private:
    // Cập nhật 1 pickup cá nhân
    void Update(float dt);

    // Vẽ 1 pickup cá nhân
    void Draw() const;

    // Dữ liệu của mỗi pickup
    Vector2    position;       // Vị trí
    Vector2    velocity;       // Vận tốc (rơi / nảy)
    float      lifeTime;       // Thời gian tồn tại
    bool       hasBounced;     // Đã nảy chưa (không dùng nữa)
    PickupType type;           // Loại pickup
    float      rotationAngle;  // Góc xoay (để xoay hình ảnh)

    // Biến static – dùng chung cho tất cả pickup
    static std::vector<Pickup> pickups;        // Danh sách các pickup
    static Texture2D texSushi, texBattery, texMilk; // Texture cho từng loại thường
    static Texture2D texGift1, texGift2, texGift3;   // Texture cho từng gift
    static float giftTimer;                   // Bộ đếm tạo gift
    static bool isLoaded;                     // Đã load texture chưa
    static bool batteryDropped;               // Đảm bảo chỉ 1 battery rơi trong 1 vòng chơi

    int bounceCount;      // Đếm số lần nảy (tối đa 2)

    static Texture2D texGlow;        // Glow effect texture

    // Hàm load texture (chỉ gọi 1 lần)
    static void LoadTextures();
};
