# 🎮 Pussy Invaders

[![C++](https://img.shields.io/badge/C++-17-blue.svg)](https://en.cppreference.com/)
[![Raylib](https://img.shields.io/badge/Raylib-4.5-green.svg)](https://www.raylib.com/)
[![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey.svg)](https://www.microsoft.com/)
[![License](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

**Pussy Invaders** là một game bắn súng không gian 2D được phát triển bằng C++ và thư viện Raylib, lấy cảm hứng từ các tựa game classic như Space Invaders và Chicken Invaders với cốt truyện độc đáo và lối chơi hấp dẫn.

## 📋 Môn học & Thành viên

* **Môn học:** Phương pháp Lập trình Hướng Đối tượng (OOP)
* **Lớp:** 24C10
* **Giảng viên hướng dẫn:** Thầy Bùi Tiến Lên
* **Nhóm thực hiện:**
    * Nguyễn Khánh Toàn – 24127252
    * Nguyễn Tiến Cường – 24127337
    * Võ Trường Hải – 24127032
    * Võ Duy Lộc – 24127441

## ✨ Tính năng nổi bật

*   **Lối chơi classic:** Bắn súng, di chuyển, né đạn và tiêu diệt kẻ thù.
*   **Hệ thống kẻ địch đa dạng:** 3 loại enemy chính (Pussy, Pussy2, Pussy3) với máu và hành vi khác nhau.
*   **Hệ thống chiến thuật:** Cơ chế bắn yêu cầu người chơi cân nhắc giữa sát thương liên tục và nguy cơ **quá nhiệt (Overheat)**.
*   **Hệ thống vật phẩm (Pickup) đa mục tiêu:** Mỗi vật phẩm (Sushi, Battery, Milk, Gift) là một quyết định chiến thuật nho nhỏ, cho phép người chơi lựa chọn hướng nâng cấp tàu của mình, từ tăng điểm số đến củng cố sức mạnh tấn công.
*   **Cốt truyện sinh động:** Intro và Outro với cốt truyện hài hước, được kể qua các scene với hiệu ứng typing.
*   **Hiệu ứng hình ảnh phong phú:** Nền xoay, hiệu ứng blink, fade, blur, slow motion, flash đỏ khi game over.
*   **Hệ thống âm thanh dynamic:** Nhạc nền thay đổi theo trạng thái game (Menu, Playing, Paused, Game Over), hiệu ứng âm thanh cho bắn, ăn, rớt vật phẩm.
*   **Giao diện người dùng (UI) trực quan:** Hiển thị đầy đủ thông số: mạng, điểm, missile, level vũ khí, thanh overheat.

## 🛠 Công nghệ sử dụng

*   **Ngôn ngữ lập trình:** C++17
*   **Thư viện đồ họa & âm thanh:** [Raylib](https://www.raylib.com/) (phiên bản 4.5+)
*   **IDE/Compiler:** Bất kỳ IDE nào hỗ trợ C++17 (Visual Studio, VS Code, Code::Blocks, etc.)
*   **Hệ điều hành:** Được phát triển và kiểm thử trên Windows.

## 🗂 Cấu trúc dự án (Project root)
```
PUSSY_INVADERS/
│
├── font/
│   ├── ChonkyBitsFontBold.otf
│   ├── ChonkyBitsFontRegular.otf
│   ├── PixelOperatorMonoHB8.ttf
│   └── PixelOperatorSC.ttf
│
├── image/
│   ├── Background-1.jpg
│   ├── Background-1.png
│   ├── bar.png
│   ├── battery.png
│   ├── bullet.png
│   ├── fireball.png
│   ├── gift-1.png
│   ├── gift-2.png
│   ├── gift-3.png
│   ├── glow_effect.png
│   ├── intro1.jpeg
│   ├── intro2.jpg
│   ├── intro3.jpg
│   ├── intro4.jpg
│   ├── intro5.jpg
│   ├── level.png
│   ├── live.png
│   ├── meteor1.png
│   ├── meteor2.png
│   ├── meteor3.png
│   ├── meteor4.png
│   ├── milk.png
│   ├── missile.png
│   ├── outro2.jpg
│   ├── outro3.png
│   ├── outro4.png
│   ├── outro5.png
│   ├── outro6.png
│   ├── pussy.png
│   ├── pussy2.png
│   ├── pussy3.png
│   ├── shit.png
│   ├── spaceship.png
│   ├── sushi.png
│   ├── thigh.png
│   ├── thunder.png
│   └── title.png
│
├── sound/
│   ├── Once Upon a Time.mp3
│   ├── background_music.mp3
│   ├── bom.mp3
│   ├── enemysound.mp3
│   ├── lasergun.wav
│   ├── meow.mp3
│   ├── pilotsound.mp3
│   └── thunder.mp3
│
├── src/
│   ├── Addition.h
│   ├── Background.cpp
│   ├── Background.h
│   ├── Brightness.cpp
│   ├── Brightness.h
│   ├── Bullet.cpp
│   ├── Bullet.h
│   ├── GameController.cpp
│   ├── GameController.h
│   ├── GameOver.cpp
│   ├── GameOver.h
│   ├── Intro.cpp
│   ├── Intro.h
│   ├── IntroTypingAudio.cpp
│   ├── IntroTypingAudio.h
│   ├── Menu.cpp
│   ├── Menu.h
│   ├── Meteor.cpp
│   ├── Meteor.h
│   ├── MusicController.cpp
│   ├── MusicController.h
│   ├── Outro.cpp
│   ├── Outro.h
│   ├── Pickup.cpp
│   ├── Pickup.h
│   ├── Pussy.cpp
│   ├── Pussy.h
│   ├── Pussy2.cpp
│   ├── Pussy2.h
│   ├── Pussy3.cpp
│   ├── Pussy3.h
│   ├── PussyBase.cpp
│   ├── PussyBase.h
│   ├── SpaceShip.cpp
│   ├── SpaceShip.h
│   ├── SpaceShipTexture.cpp
│   ├── SpaceShipTexture.h
│   ├── ThunderBullet.cpp
│   ├── ThunderBullet.h
│   └── main.cpp
```
## 🚀 Hướng dẫn Chạy Game

### Cách 1: Chạy trực tiếp (Dành cho người dùng cuối)

1.  Vào thư mục `RUN_HERE`.
2.  Nhấp đúp chuột vào file `PUSSY_INVADERS.exe` để chạy game.

### Cách 2: Biên dịch từ mã nguồn (Dành cho nhà phát triển)

1.  **Cài đặt môi trường:**
    *   Cài đặt một IDE hỗ trợ C++ (khuyến nghị **Visual Studio 2022**).
    *   Cài đặt thư viện **Raylib**. Có thể sử dụng `vcpkg`:
        ```bash
        vcpkg install raylib:x64-windows
        ```
    *   Hoặc tải về từ trang chủ và thiết lập thủ công: [https://www.raylib.com/](https://www.raylib.com/)

2.  **Biên dịch project:**
    *   Mở project trong IDE của bạn.
    *   Thiết lập include path và library path để trỏ đến thư viện Raylib.
    *   Thêm các file `.cpp` vào project.
    *   Build project. File thực thi sẽ được tạo ra.
    *   **Đảm bảo** copy các thư mục `audio/`, `images/`, `font/` đến cùng thư mục với file thực thi.

## 🎮 Điều khiển

| Hành động | Điều khiển |
| :--- | :--- |
| **Di chuyển tàu** | Di chuyển chuột hoặc sử dụng phím **W/A/S/D** (hoặc **Mũi tên**) |
| **Bắn đạn** | Nhấn **Space** |
| **Tạm dừng/Tiếp tục** | Nhấn **P** hoặc **ESC** |
| **Skip Intro/Outro** | Nhấn **Enter** |
| **Ăn vật phẩm** | Tự động khi tàu chạm vào vật phẩm |

## 📖 Báo cáo & Tài liệu

*   **Báo cáo đầy đủ:** Xem file `[group]-[student-ids].pdf` trong thư mục `report/` để biết chi tiết về thiết kế UML, phân tích OOP, Design Patterns, cốt truyện, và đánh giá dự án.
*   **Mã nguồn:** Được documented trong thư mục src.

## 🔮 Hướng phát triển trong tương lai

*   Thêm nhiều loại đạn và vũ khí đặc biệt.
*   Thiết kế thêm nhiều màn chơi (level) với độ khó tăng dần.
*   Phát triển hệ thống Boss cuối với nhiều phase.
*   Bổ sung chế độ chơi phụ (challenge mode).
*   Cải thiện AI cho kẻ địch, giúp chúng có hành vi phức tạp hơn.

## 👥 Đóng góp

Dự án được phát triển bởi nhóm sinh viên lớp 24C10, khoa CNTT, Trường Đại học Khoa học Tự nhiên, ĐHQG-HCM.

## 📄 Giấy phép

Dự án này được phân phối theo giấy phép MIT. Xem file `LICENSE` để biết thêm thông tin.

---

**Chúc bạn chơi game vui vẻ! Meow! 🐱👾**
