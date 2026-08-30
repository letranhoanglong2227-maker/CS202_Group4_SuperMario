# Hướng dẫn tích hợp Shared Asset Resolver (Từ Person 4)

Chào mọi người, dựa theo yêu cầu cấp bách (Critical Path) nhằm chuẩn hóa việc load tài nguyên và dọn đường cho khâu đóng gói (Package) sau này, tôi (Person 4) đã hoàn thiện class `AssetResolver`.

Tài liệu này hướng dẫn mọi người cách chuyển đổi từ kiểu load file cũ sang hệ thống mới. Xin lưu ý **áp dụng ngay** cho các Texture/Sound Manager mà các bạn đang phụ trách.

---

## 1. Tại sao phải đổi?
Trước đây, chúng ta hay fix cứng (hardcode) đường dẫn hoặc dùng fallback kiểu dò ngược thư mục:
```cpp
// ❌ CÁCH CŨ (DỄ GÂY LỖI)
bool ok = texture.loadFromFile("assets/textures/player.png");
if (!ok) {
    ok = texture.loadFromFile("../assets/textures/player.png");
}
```
**Nhược điểm:** Cách này phụ thuộc vào `Working Directory` của từng máy. Khi chạy qua Visual Studio, CLion, qua CTest hay lúc click đúp vào file `.exe` khi release, thư mục làm việc sẽ khác nhau và game sẽ bị màn hình đen hoặc văng lỗi âm thầm.

**Giải pháp mới:** `AssetResolver` sẽ tự động lấy vị trí gốc của file `.exe` đang chạy và tìm thư mục `assets` ở ngay bên cạnh nó (Executable-adjacent). Dù bạn chạy game kiểu gì, nó vẫn luôn tìm đúng chỗ!

---

## 2. Cách sử dụng (Before & After)

### Bước 1: Include thư viện
Thêm dòng này vào đầu file Manager của bạn:
```cpp
#include "Core/AssetResolver.hpp"
#include <iostream>
```

### Bước 2: Bọc code load file bằng try-catch
Hệ thống mới được thiết kế theo chuẩn **Fail-fast** (Sai là báo ngay). Nếu không tìm thấy file, hàm `resolve()` sẽ ném ra (throw) một `std::runtime_error` với thông báo cực kì rõ ràng.

#### 👉 Code thay thế mẫu:

```cpp
// ✅ CÁCH MỚI (CHUẨN)
static bool setupTexture() {
    try {
        // 1. Dùng AssetResolver để lấy đường dẫn tuyệt đối tĩnh
        auto path = AssetResolver::resolve("assets/textures/Player.png");
        
        // 2. Nạp vào SFML bằng path.string()
        bool loadOk = playerTexture.loadFromFile(path.string());
        
        if (loadOk) {
            std::cout << "[SUCCESS] Nạp thành công: " << path.string() << "\n";
        }
        return loadOk;

    } catch (const std::exception& e) {
        // 3. Xử lý khi không tìm thấy file
        std::cerr << "[ERROR] Lỗi nạp tài nguyên: " << e.what() << "\n";
        return false;
    }
}
```

---

## 3. Các lưu ý quan trọng
1. **Không tự chế Fallback:** Bạn chỉ cần truyền ĐÚNG một chuỗi đường dẫn tương đối tính từ thư mục gốc (vd: `"assets/textures/..."` hoặc `"assets/audio/..."`). Tuyệt đối không tự viết thêm logic fallback dò ngược `../` nữa.
2. **Path separator:** Nên dùng dấu `/` thay vì `\\` để C++ tự động cross-platform.
3. **Thống nhất báo lỗi:** Bắt buộc phải có `try...catch` khi dùng `AssetResolver::resolve()`. Nếu file bị thiếu, game sẽ báo lỗi in ra màn hình console và trả về `false`, giúp team ta debug nhanh hơn nhiều so với việc ngồi đoán xem file bị thiếu ở đâu.

Mọi người vui lòng update các class Manager (`TexturePlayerManager`, `TextureEnemyManager`, âm thanh, v.v.) của mình theo mẫu trên nhé. Chúc team code mượt!
