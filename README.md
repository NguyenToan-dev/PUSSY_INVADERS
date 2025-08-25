# 🎮 OOP FINAL PROJECT: GAME PUSSY INVADERS

## 📚 Course Info

* **Môn học**: Phương pháp lập trình hướng đối tượng
* **Lớp**: 24C10
* **Nhóm thực hiện**:

  * Nguyễn Khánh Toàn – MSSV: 24127252
  * Nguyễn Tiến Cường – MSSV: 24127337
  * Võ Trường Hải     – MSSV: 24127032
  * Võ Duy Lộc        – MSSV: 24127441
---

## 🔎 Overview
Pussy Invaders là một game bắn súng không gian 2D với đồ họa đẹp mắt và lối chơi hấp dẫn. Người chơi điều khiển một phi thuyền chiến đấu chống lại các chú mèo xâm lược (Pussy Invaders) qua nhiều đợt tấn công khác nhau.

## Tính năng chính
* Hệ thống cấp độ: 3 loại kẻ địch với độ khó tăng dần (Pussy, Pussy2, Pussy3)
* Hệ thống vũ khí: Nhiều loại đạn khác nhau, có thể nâng cấp
* Thu thập vật phẩm: Sushi, Battery, Milk và các gift item đặc biệt
* Hiệu ứng hình ảnh: Blur, flash, slow motion, và nhiều hiệu ứng chuyển cảnh
* Hệ thống âm thanh: Nhạc nền dynamic, hiệu ứng âm thanh phong phú
* Cốt truyện: Phần intro và outro với cốt truyện hấp dẫn

## Công nghệ sử dụng
* Raylib: Framework đồ họa và âm thanh
* C++: Ngôn ngữ lập trình chính

## Cấu trúc dự án
### Lớp chính
* GameController: Điều khiển luồng chính của game
* Menu: Hệ thống menu chính và cài đặt
* Intro/Outro: Phần giới thiệu và kết thúc game

### Nhân vật
* SpaceShip: Phi thuyền của người chơi
* PussyBase/Pussy/Pussy2/Pussy3: Các loại kẻ địch
* Meteor: Thiên thạch ngẫu nhiên

### Vũ khí và vật phẩm
* Bullet/ThunderBullet: Các loại đạn
* Pickup: Hệ thống vật phẩm có thể thu thập

### Hiệu ứng và đồ họa
* Background: Hệ thống background xoay
* Brightness: Điều chỉnh độ sáng
* GameOver: Xử lý màn hình game over

### Âm thanh
* MusicController: Điều khiển nhạc nền
* IntroTypingAudio: Tạo hiệu ứng âm thanh gõ chữ

## Hướng dẫn biên dịch
* Cài đặt Raylib phiên bản mới nhất
* Clone repository này
* Biên dịch tất cả file .cpp và liên kết với thư viện Raylib
* Chạy file thực thi

## Điều khiển
* Di chuyển: Chuột hoặc phím mũi tên
* Bắn: Click chuột trái hoặc phím cách
* Tạm dừng: Phím P hoặc ESC
