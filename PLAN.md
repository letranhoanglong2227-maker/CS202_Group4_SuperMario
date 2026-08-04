# 📅 KẾ HOẠCH PHÁT TRIỂN CHI TIẾT - SUPER MARIO C++ (4 TUẦN)
**Phiên bản:** 4.0 (Bản đặc tả tối ưu: Tách biệt Code Parser Map và Thiết Kế Map)

---

## 🏗️ 1. HƯỚNG DẪN KIẾN TRÚC & TIÊU CHUẨN KỸ THUẬT (MANDATORY)

Để dự án không bị loãng và tránh xung đột code khi làm việc nhóm, toàn bộ 4 thành viên phải tuân thủ nghiêm ngặt các quy tắc kỹ thuật sau:

### 1.1 Quản lý Bộ nhớ & Quyền Sở hữu (Memory Ownership)
*   **Không sử dụng con trỏ thô (Raw Pointers) để quản lý vòng đời đối tượng.** Sử dụng `std::unique_ptr` làm mặc định cho các đối tượng đơn lẻ và `std::vector<std::unique_ptr<T>>` để quản lý danh sách thực thể.
*   `LevelManager` giữ quyền sở hữu tuyệt đối đối với danh sách thực thể (`std::vector<std::unique_ptr<GameObject>>`).
*   Các hệ thống khác chỉ nhận tham chiếu (`T&`) hoặc con trỏ quan sát (`T*` không sở hữu) để xử lý logic.

### 1.2 Phương Pháp Xây Dựng Bản Đồ (Color-Coded Image-Based Level Loading)
*   **Chiến lược tách biệt:**
    - **Giai đoạn Code (Tuần 1 - Tuần 3):** Team lập trình chỉ dùng **một file ảnh map thử nghiệm duy nhất (`test_map.png`)** kích thước nhỏ (ví dụ 40x15 pixel) để chạy thử nghiệm các tính năng di chuyển, nhảy, va chạm và sinh quái.
    - **Giai đoạn Thiết kế Map (Tuần 4):** Sau khi đã chốt thông số vật lý (Mario nhảy cao bao nhiêu ô, chạy nhanh thế nào), team mới tiến hành vẽ và hoàn thiện 9 file ảnh map chính thức (`W1_LV1.png` đến `W3_LV3.png`). Điều này giúp tránh việc phải vẽ đi vẽ lại bản đồ nhiều lần khi thông số vật lý thay đổi.
*   **Nguyên lý hoạt động của `MapManager`:**
    1.  Nạp bức ảnh bản đồ vào một đối tượng `sf::Image` (đọc màu từng pixel thông qua hàm `sf::Image::getPixel(x, y)`).
    2.  Duyệt qua lưới pixel của ảnh theo trục X và Y để xác định tọa độ thực tế: `World_X = Pixel_X * TILE_SIZE`, `World_Y = Pixel_Y * TILE_SIZE` (mặc định `TILE_SIZE = 32`).
    3.  **Bảng Ánh Xạ Màu Sắc Quy Ước:**
        - `sf::Color::Black` (0, 0, 0): Tạo `SolidBlock` (Gạch nền đất cứng).
        - `sf::Color::Red` (255, 0, 0): Vị trí bắt đầu của Player 1 (Mario).
        - `sf::Color::Green` (0, 255, 0): Vị trí bắt đầu của Player 2 (Luigi).
        - `sf::Color::Blue` (0, 0, 255): Sinh kẻ địch `Goomba`.
        - `sf::Color::Yellow` (255, 255, 0): Sinh `CoinBlock` (Khối chấm hỏi chứa xu).
        - `sf::Color::Magenta` (255, 0, 255): Sinh `MushroomBlock` (Khối chấm hỏi chứa nấm).
        - `sf::Color(128, 64, 0)` (Nâu): Tạo gạch thường có thể vỡ `Brick`.
        - `sf::Color::Transparent` hoặc `sf::Color::White`: Khoảng không khí trống.

---

## 📅 2. TIẾN ĐỘ THỰC HIỆN CHI TIẾT (4 TUẦN)

---

### 📅 TUẦN 1: CORE ENGINE, STATE PATTERN, OOP BASE & TRÌNH ĐỌC MAP THEO PIXEL ẢNH
**Mục tiêu:** Chạy được Game Loop cơ bản, `MapManager` đọc được file ảnh thử nghiệm `test_map.png` vẽ ra lưới debug, nhân vật rơi chạm đất đứng vững.

#### 👤 Người 1: Game Loop, State Pattern & Giao Diện Menu
*   **File phụ trách:** `Core/MyApp.hpp`, `Core/MyApp.cpp`, `Core/stdafx.hpp`, `States/Base/State.hpp`, `States/Base/GameState.hpp`, `States/Menus/MainMenuState.hpp`.
*   **Nhiệm vụ cụ thể:**
    1.  Cài đặt Game Loop chuẩn trong `MyApp.cpp` với Delta Time.
    2.  Triển khai **State Pattern**: Viết logic base cho `State.hpp` chứa các hàm ảo `init()`, `handleInput()`, `update(dt)`, `draw(window)`.
    3.  Tạo lớp `GameState` kế thừa `State` làm nơi quản lý màn chơi chính.
    4.  Cài đặt `MainMenuState` cơ bản cho phép nhấn Enter để chuyển trạng thái sang `GameState`.

#### 👤 Người 2: Physics Base, Movement Component & Viết Code Trình Đọc Bản Đồ (Map Parser)
*   **File phụ trách:** `Physics/PhysicsEngine.hpp`, `Physics/PhysicsEngine.cpp`, `Components/MovementComponent.hpp`, `Components/MovementComponent.cpp`, `Levels/Managers/MapManager.hpp`, `Levels/Managers/MapManager.cpp`.
*   **Nhiệm vụ cụ thể:**
    1.  Định nghĩa `MovementComponent`: Quản lý vận tốc, gia tốc, ma sát.
    2.  Xây dựng `PhysicsEngine` cơ bản: Gia tốc trọng lực (gravity) rơi tự do.
    3.  **Trình đọc bản đồ theo pixel (Code Map Parser):** Viết logic nạp ảnh bằng `sf::Image` và duyệt pixel. Áp dụng Grid/Spatial Partitioning để giới hạn phạm vi xét va chạm, tối ưu FPS.
    4.  **Tạo file ảnh test:** Vẽ một ảnh `test_map.png` siêu nhỏ (ví dụ chỉ có nền đen nằm ngang ở dưới làm đất, một chấm đỏ ở trên làm người chơi) để test code. Vẽ viền đỏ debug các hitbox.

#### 👤 Người 3: OOP Entity Hierarchy & Player Logic cơ bản
*   **File phụ trách:** `Core/GameObject.hpp`, `Entities/Base/LivingEntity.hpp`, `Entities/Base/Character.hpp`, `Entities/Players/Mario.hpp`.
*   **Nhiệm vụ cụ thể:**
    1.  Xây dựng `GameObject`: Chứa `sf::FloatRect hitbox`, vị trí, kích thước, và trạng thái sống.
    2.  Thiết lập `LivingEntity` và `Character` kế thừa từ `GameObject`.
    3.  Tạo `Mario` liên kết `MovementComponent` để xử lý phím di chuyển trái phải và nhảy trên Hitbox ảo.

#### 👤 Người 4: Texture Atlas Slicing & Base Blocks
*   **File phụ trách:** `Components/AnimationComponent.hpp`, `Components/AnimationComponent.cpp`, `Objects/Blocks/Block.hpp`, `Objects/Blocks/SolidBlock.hpp`, `Core/UserData.hpp`.
*   **Nhiệm vụ cụ thể:**
    1.  **Cắt ảnh từ Master Spritesheet:** Viết class `AnimationComponent` quản lý `sf::Sprite` và hàm `setAnimationFrame` cắt ảnh từ file texture tổng.
    2.  Xây dựng `Block` kế thừa `GameObject`, và `SolidBlock` kế thừa `Block` (đại diện cho gạch đất đá tĩnh cứng).
    3.  Định nghĩa `UserData` lưu điểm, mạng sống, tên.

---

### 📅 TUẦN 2: CHẾ ĐỘ MULTIPLAYER, VA CHẠM AABB, KẺ ĐỊCH CẤP 1 & FACTORY (TEST MAP)
**Mục tiêu:** Chọn được 1P/2P, cả hai cùng xuất hiện trên màn hình thông qua file `test_map.png` có thêm chấm màu xanh lá, va chạm đất đá chuẩn xác không kẹt tường.

#### 👤 Người 1: Multiplayer Character Selection & Player Manager
*   **File phụ trách:** `States/Menus/MenuCharacterSelectionState.hpp`, `Entities/Players/PlayerManager.hpp`, `Entities/Players/PlayerManager.cpp`, `States/Menus/PauseMenuState.hpp`.
*   **Nhiệm vụ cụ thể:**
    1.  Cài đặt `MenuCharacterSelectionState`: Cho phép chọn chế độ 1 Player hoặc 2 Players.
    2.  Xây dựng `PlayerManager`: Quản lý danh sách 1 hoặc 2 người chơi hiện hoạt và ánh xạ phím bấm cho mỗi người.
    3.  Implement `PauseMenuState` để tạm dừng game khi nhấn ESC.

#### 👤 Người 2: Thuật Toán Va Chạm Toàn Diện & Tích Hợp Obstacles Lên Map Test
*   **File phụ trách:** `Physics/PhysicsEngine.cpp` (hàm va chạm), `Objects/Environment/Pipe.hpp`, `Objects/Environment/Lava.hpp`, `Levels/Managers/LevelManager.hpp`.
*   **Nhiệm vụ cụ thể:**
    1.  Hoàn thiện va chạm AABB trong `PhysicsEngine.cpp`: Áp dụng Axis-Separated Collision (tách trục X, Y) để xử lý kẹt tường, trượt góc, tránh lỗi kẹt mép gạch.
    2.  Tạo lớp `Pipe` (ống nước) và `Lava` (dung nham).
    3.  **Cập nhật `test_map.png`:** Thêm pixel màu ống nước và dung nham vào ảnh map test để chạy thử nghiệm va chạm.

#### 👤 Người 3: Khởi Tạo Thực Thể (Factory Pattern), Kẻ Địch Cấp 1 & Items
*   **File phụ trách:** `Entities/EntityFactory.hpp`, `Entities/EntityFactory.cpp`, `Entities/Enemies/Goomba.hpp`, `Entities/Enemies/Koopa.hpp`, `Objects/Items/Item.hpp`, `Objects/Items/Coin.hpp`, `Objects/Items/Mushroom.hpp`.
*   **Nhiệm vụ cụ thể:**
    1.  Hoàn thiện **Factory Pattern** tại `EntityFactory.cpp` để sinh quái/vật phẩm khi parser đọc được màu tương ứng trên ảnh test.
    2.  Cài đặt AI tuần tra cơ bản cho `Goomba` và `Koopa` (đảo hướng khi đụng tường).
    3.  Định nghĩa logic cho `Item`: Nấm `Mushroom` và Đồng tiền `Coin`.

#### 👤 Người 4: Hộp Quà Tương Tác & Animation Chuyển Động Động
*   **File phụ trách:** `Objects/Blocks/CoinBlock.hpp`, `Objects/Blocks/MushroomBlock.hpp`, `Objects/Blocks/Brick.hpp`, `Components/AnimationComponent.cpp` (phần update frame).
*   **Nhiệm vụ cụ thể:**
    1.  Xây dựng `Brick` (phá vỡ được) và các hộp quà tương tác: `CoinBlock`, `MushroomBlock`.
    2.  Nâng cấp `AnimationComponent` xoay vòng frame ảnh tạo hoạt ảnh chạy bộ cho Mario/Luigi.

---

### 📅 TUẦN 3: EVENT SYSTEM (MEDIATOR), DYNAMIC INTERACTION, BOSS LEVEL 2 & AUDIO
**Mục tiêu:** Mario đập đầu vào gạch nảy đồ lên hoặc vỡ vụn, ăn nấm to ra, bắn sự kiện chết về trung tâm, nạp World 2 bằng ảnh vẽ.

#### 👤 Người 1: Hệ Thống Sự Kiện (Mediator Pattern) & Menu Nhập Tên
*   **File phụ trách:** `Core/GameEventMediator.hpp`, `Core/GameEventMediator.cpp`, `States/Menus/EnterNameState.hpp`, `States/Menus/SettingState.hpp`.
*   **Nhiệm vụ cụ thể:**
    1.  Cài đặt **Mediator Pattern**: `GameEventMediator` quản lý các sự kiện toàn cục (`PlayerDied`, `ScoreChanged`, `LevelFinished`).
    2.  Cài đặt `EnterNameState` nhập tên người chơi và `SettingState` cấu hình tùy chọn âm lượng.

#### 👤 Người 2: Phản Hồi Vật Lý Động & Cơ Chế Lò Xo/Platform Di Chuyển
*   **File phụ trách:** `Physics/PhysicsEngine.cpp` (phần va chạm động), `Objects/Environment/Trampoline.hpp`, `Objects/Blocks/MovingBlock.hpp`.
*   **Nhiệm vụ cụ thể:**
    1.  Lập trình va chạm từ phía dưới (Headbutt) trong `PhysicsEngine`: Phá vỡ `Brick` hoặc nảy `CoinBlock` đẩy vật phẩm trượt ra ngoài.
    2.  Tạo lò xo `Trampoline` đẩy cao và `MovingBlock` di chuyển tuần hoàn trục ngang/dọc để phục vụ các địa hình khó sau này.

#### 👤 Người 3: Quái Vật Bay, Buff & Debuff Hệ Thống
*   **File phụ trách:** `Entities/Enemies/FlyingKoopa.hpp`, `Entities/Enemies/Heriss.hpp`, `Objects/Items/PowerUpObject.hpp`, `Objects/Items/FireBuff.hpp`, `Objects/Items/PlayerBuff.hpp`.
*   **Nhiệm vụ cụ thể:**
    1.  Cài đặt quái vật bay `FlyingKoopa` và quái Nhím `Heriss`.
    2.  Phát triển logic Buff: Ăn `Mushroom` -> Kích hoạt `PlayerBuff` phóng to Hitbox; Ăn Hoa lửa -> Kích hoạt `FireBuff` bắn đạn.

#### 👤 Người 4: Audio System (Singleton), Hiệu Ứng Vỡ Gạch & HUD GUI
*   **File phụ trách:** `Audio/AudioSystem.hpp`, `Audio/AudioSystem.cpp`, `Objects/Blocks/BrickFragment.hpp`, `UI/GUI.hpp`, `UI/LevelGUI.hpp`.
*   **Nhiệm vụ cụ thể:**
    1.  Implement **Singleton Pattern** cho `AudioSystem` nạp nhạc và phát âm thanh SFX.
    2.  Tạo hiệu ứng văng mảnh gạch vỡ `BrickFragment`.
    3.  Thiết kế HUD `LevelGUI` hiển thị điểm, số mạng và thời gian.

---

### 📅 TUẦN 4: THIẾT KẾ & VẼ 9 BẢN ĐỒ, BOSS BATTLE, BẢNG CSV & HOÀN THIỆN ĐỒ HỌA
**Mục tiêu:** Thiết kế/Vẽ 9 ảnh map bằng PNG, tích hợp Boss, lưu file Leaderboard và đóng gói đồ án.

#### 👤 Người 1: Flow Chuyển World, Kết Thúc Game & Menu Hoàn Chỉnh
*   **File phụ trách:** `States/Menus/MenuWorldState.hpp`, `States/Menus/MenuLevelState.hpp`, `States/Menus/DeathMenuState.hpp`, `States/Menus/WinMenuState.hpp`.
*   **Nhiệm vụ cụ thể:**
    1.  Xây dựng `MenuWorldState` và `MenuLevelState` (chọn World 1, 2, 3), kiểm soát màn khóa/mở dựa trên tiến trình đã lưu trong `SaveData/`.
    2.  Hoàn thiện các Menu kết thúc game (`DeathMenuState`, `WinMenuState`).

#### 👤 Người 2: THIẾT KẾ & VẼ 9 BẢN ĐỒ PNG (World 1 - 3), Vũ Khí Môi Trường & Cột Cờ
*   **File phụ trách:** `Objects/Environment/Cannon.hpp`, `Objects/Environment/Bullet.hpp`, `Objects/Environment/Rocket.hpp`, `Objects/Environment/WinFlag.hpp`, `Levels/Stages/W1_LV1.hpp` -> `W3_LV3.hpp`.
*   **Nhiệm vụ cụ thể:**
    1.  **Thiết kế & Vẽ map:** Tiến hành dùng công cụ vẽ để thiết kế toàn bộ 9 màn chơi (`W1_LV1.png` đến `W3_LV3.png`) dựa trên bảng mã màu. Bố trí các chướng ngại vật hợp lý dựa trên độ cao nhảy của Mario đã chốt ở tuần 3.
    2.  Tạo bệ pháo `Cannon` bắn đạn đi ngang, tên lửa đuổi `Rocket` và cột cờ chiến thắng `WinFlag` ở cuối mỗi map.

#### 👤 Người 3: Trận Đấu Boss & Cân Bằng Gameplay
*   **File phụ trách:** `Entities/Enemies/Bowser.hpp`, `Entities/Enemies/PeteyPiranha.hpp`.
*   **Nhiệm vụ cụ thể:**
    1.  Phát triển Boss rồng lửa `Bowser` (ở màn `W3_LV3` khè lửa, nhảy dậm đất) và Boss hoa ăn thịt `PeteyPiranha` (phun hạt gai trồi sụt ống nước).
    2.  Tiến hành chơi thử nghiệm cả 9 màn chơi để cân bằng độ khó, căn chỉnh tốc độ và máu của Boss.

#### 👤 Người 4: Đọc Ghi CSV Xếp Hạng, Save/Load Game & Đóng Gói
*   **File phụ trách:** `States/Menus/LeaderboardState.hpp`, `LeaderBoard.csv`.
*   **Nhiệm vụ cụ thể:**
    1.  Cấu hình đọc/ghi dữ liệu bảng xếp hạng kỷ lục vào file `LeaderBoard.csv`.
    2.  Lưu tiến trình người chơi vào `SaveData/`.
    3.  Tối ưu hóa hình ảnh Spritesheet tổng thể và thực hiện đóng gói game (CMake Release build).

---

## 📊 3. BẢNG PHÂN CHIA VÀ TỔNG KẾT KHỐI LƯỢNG CÔNG VIỆC

| Thành Viên | Trách Nhiệm Chính | Các File .hpp/.cpp Phụ Trách Chính | Các Design Pattern Áp Dụng |
| :---: | :--- | :--- | :--- |
| **Người 1** | **Core Loop, Game States Flow, Event Mediator & UI Menu** | `MyApp`, `stdafx`, `State`, `GameState`, `MainMenuState`, `MenuCharacterSelectionState`, `PauseMenuState`, `SettingState`, `EnterNameState`, `MenuWorldState`, `MenuLevelState`, `DeathMenuState`, `WinMenuState`, `GameEventMediator`. | **State, Mediator** |
| **Người 2** | **Physics Engine, Va Chạm, Code Map Parser & Vẽ Thiết Kế 9 Màn Chơi** | `PhysicsEngine`, `MovementComponent`, `LevelManager`, `MapManager`, `W1_LV1` -> `W3_LV3`, `Pipe`, `Lava`, `Trampoline`, `MovingBlock`, `Cannon`, `Bullet`, `Rocket`, `WinFlag`. | **Component (Physics)** |
| **Người 3** | **Entities Hierarchy, Enemy AI, Buff System & Bosses** | `GameObject`, `LivingEntity`, `Character`, `Mario`, `Luigi`, `PlayerManager`, `EntityFactory`, `Goomba`, `Koopa`, `FlyingKoopa`, `Heriss`, `Bowser`, `PeteyPiranha`, `Item`, `Coin`, `Mushroom`, `PowerUpObject`, `FireBuff`, `PlayerBuff`. | **Factory** |
| **Người 4** | **Graphics Render, Audio, Blocks & Save System (IO)** | `AnimationComponent`, `AudioSystem`, `Block`, `SolidBlock`, `CloudBlock`, `Brick`, `CoinBlock`, `MushroomBlock`, `BrickFragment`, `UserData`, `GUI`, `LevelGUI`, `LeaderboardState` (đọc ghi `LeaderBoard.csv`). | **Component (Render), Singleton** |
