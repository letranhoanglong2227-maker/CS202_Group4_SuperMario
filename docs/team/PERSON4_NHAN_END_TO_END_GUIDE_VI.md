# Hướng dẫn Person 4 (Nhân) từ đầu đến cuối

> Người thực hiện: **Nhân — Person 4**
>
> Repo: `SUPERMARIO/CS202_Group4_SuperMario`
>
> Phạm vi: animation component, block presentation/behavior, GUI/HUD, audio, dữ liệu lưu, leaderboard và đóng gói bản chạy.
>
> Cập nhật theo trạng thái source và tài liệu ngày **2026-08-30**.

---

## 1. Nhân cần hiểu đúng việc của mình trước khi code

Person 4 sở hữu **phần trình bày, dữ liệu và release**, cụ thể:

- `AnimationComponent`.
- Các block do P4 quản lý: `Block`, `SolidBlock`, `Brick`, `BrickFragment`, `CoinBlock`, `MushroomBlock`, `CloudBlock`.
- `AudioSystem`.
- `GUI` và `LevelGUI`.
- `UserData` và persistence.
- `LeaderboardState` và dữ liệu leaderboard.
- `CMakeLists.txt`, asset copy/install và package ở giai đoạn release.

Nhân **không** sở hữu:

- P1: `main.cpp`, `MyApp`, `GameEventMediator`, State và menu flow; riêng `LeaderboardState` là P4.
- P2: `LevelManager`, `MapManager`, `PhysicsEngine`, stage, map parsing, `MovingBlock`, Pipe, Lava, Trampoline, Cannon, Bullet, Rocket, WinFlag.
- P3: `GameObject`, player, enemy, item, buff và `EntityFactory`.
- Nội dung pixel của map PNG.

Nguyên tắc tích hợp:

> P4 cung cấp API và kết quả trình bày/dữ liệu. P1 quyết định state/session. P2 phát hiện collision và sở hữu runtime object. P3 quyết định hành vi entity/item. Nếu thiếu API, gửi XREQ; không sửa chéo file để chạy tạm.

---

## 2. Trạng thái thật tại thời điểm bắt đầu

### 2.1 Baseline đã hoàn thành, không làm lại

| Card | Trạng thái | Điều phải giữ nguyên |
|---|---|---|
| `BASE-P4-BRICK-001` | `DONE` | Brick thường chỉ vỡ một lần khi hit hợp lệ, tạo đúng bốn fragment |
| `BASE-P4-FRAGMENT-001` | `DONE` | Fragment được đặt đúng vị trí, update/expire và được P2 nhận ownership |
| `BASE-P4-BLOCK-INTEGRATION-001` | `DONE` | Collision constant dùng chung, `isExist()` public, Coin popped và Mushroom spawn trên block một tile |

Lưu ý về `BrickFragment`: lỗi cũ là gọi `setPosition` nhưng sprite không đồng bộ đúng và lifecycle chưa nối hoàn chỉnh. Lỗi này đã được sửa và kiểm thử trong baseline. Nhân không rewrite `BrickFragment` chỉ vì thấy implementation chưa đẹp. Nếu file này bị chạm bởi task khác, phải chạy lại regression Brick/Fragment.

### 2.2 Task có thể làm ngay

| Thứ tự | Card | Mức ưu tiên | Trạng thái |
|---:|---|---|---|
| 1 | `P4-ANIMATION-001` | P0 | `READY` |
| 2 | `P4-BLOCK-SIZE-001` | P0 | `READY` |
| 3 | `P4-GUI-001` | P0 | `READY` |
| 4 | `P4-QUESTION-BLOCK-001` | P1 | `READY` |
| 5 | `P4-AUDIO-001` | P1 | `READY` |
| 6 | phần in-memory nine-stage của `P4-PERSISTENCE-001` | P0 provider | Có thể làm trước |

### 2.3 Task chưa được tự ý bắt đầu

| Card | Vì sao chưa làm trọn vẹn |
|---|---|
| `P4-PAYLOAD-BLOCK-001` | Chờ P2 forward variant và P3 cung cấp payload 1-Up/Star/API item |
| `P4-HUD-001` | Chờ P1 chốt cấu trúc snapshot/event cụ thể |
| `P4-PERSISTENCE-001` durable I/O | Chờ event surface của P1 và writable-path integration |
| `P4-LEADERBOARD-001` | Làm sau GUI + persistence + P1 routing |
| `P4-PACKAGE-001` | Làm cuối, sau các runtime task và shared asset resolver |
| `P4-CLOUD-001` | `DEFERRED/OUT_OF_CURRENT_SCOPE`; không implement trong release hiện tại |

Tám quyết định trong `03_HUMAN_DECISIONS.md` đều đã `RESOLVED`. Tuy nhiên quyết định đã chốt không có nghĩa mọi dependency code đã tồn tại. Task vẫn `BLOCKED` nếu provider chưa giao API hoặc integration chưa sẵn sàng.

### 2.4 Những điểm source hiện tại cần biết

- Chưa có remote branch `origin/person4`.
- Baseline tích hợp mới nhất đang ở `origin/person2/map-integration`, commit đã quan sát: `ecb18cb`.
- `AnimationComponent` đã có nhưng duplicate/missing key chỉ in log, empty frame và interval không hợp lệ chưa có contract, `dt` lớn làm mất thời gian dư.
- `Block::setSizeBlock()` có scale sprite nhưng P2 gọi virtual `GameObject::setSize()`, nên seam visual 16×16 → world 32×32 chưa được đóng đúng.
- `CoinBlock` và `MushroomBlock` đã spawn item ở `Y - 32` và dùng popped state. Không được làm mất correction này.
- `AudioSystem`, `GUI`, `LevelGUI`, `LeaderboardState` hiện là shell rỗng.
- `UserData` mới là in-memory model, chưa validate/serialize.
- CMake hiện đã tìm SFML 3.1 và fallback fetch tag `3.1.0`; phần copy asset/package vẫn chưa hoàn thành. Dòng mô tả cũ nói pin SFML 3.0 trong execution plan đã lỗi thời so với source hiện tại.
- `TextureBlockManager` hiện thử hai đường dẫn theo working directory. Quyết định cuối là một asset root cạnh executable; không tiếp tục thêm fallback thứ ba, thứ tư ở từng class.

---

## 3. Bước 0 — Đọc tài liệu theo đúng thứ tự

Từ thư mục repo, Nhân đọc:

1. `docs/team/03_HUMAN_DECISIONS.md` — quyết định đã được duyệt.
2. `docs/team/02_INTEGRATION_CONTRACTS.md` — ranh giới API giữa P4 và P1/P2/P3.
3. `docs/team/01_MASTER_TASK_BOARD.md` — status hiện hành.
4. `docs/team/P4_EXECUTION_PLAN.md` — checklist chi tiết từng task.
5. `docs/team/04_USAGE_AND_PROGRESS_WORKFLOW.md` — cách cập nhật tiến độ và gửi cross-owner request.

Không lấy một đoạn `BLOCKED` cũ trong execution plan làm nguồn duy nhất. Thứ tự ưu tiên khi có mâu thuẫn:

1. Source hiện tại.
2. Decision đã duyệt.
3. Integration contract.
4. Master board.
5. Execution plan/audit cũ.

---

## 4. Bước 1 — Tạo branch Person 4 an toàn

Mở PowerShell tại thư mục `Do`:

```powershell
cd '.\SUPERMARIO\CS202_Group4_SuperMario'
git status --short --branch
git fetch origin
```

Nếu chưa có local branch `person4`:

```powershell
git switch -c person4 origin/person2/map-integration
git push -u origin person4
```

Nếu đã có local branch `person4`:

```powershell
git switch person4
git status --short --branch
git merge --no-ff origin/person2/map-integration
```

Nếu `git status` có file chưa commit của Nhân, dừng merge và commit đúng task hoặc dùng stash có tên rõ ràng:

```powershell
git stash push -u -m 'person4-wip-before-p2-integration'
```

Không dùng:

```text
git reset --hard
git checkout -- <file>
```

nếu chưa chắc file đó có thay đổi của mình hay đồng đội.

Sau khi branch được tạo, xác nhận:

```powershell
git status --short --branch
git log --oneline --decorate -n 8
```

Kết quả mong đợi: đang ở `person4`, working tree sạch, lịch sử có baseline P2/P4 đã kiểm thử.

---

## 5. Bước 2 — Chạy baseline trước khi sửa

Tạo build directory riêng, không dùng build directory của P2:

```powershell
cmake -S . -B build-person4
cmake --build build-person4 --config Debug --target Person2RuntimeContracts
ctest --test-dir build-person4 -C Debug --output-on-failure
```

Nếu dùng Ninja:

```powershell
cmake -S . -B build-person4-ninja -G Ninja
cmake --build build-person4-ninja --target Person2RuntimeContracts
ctest --test-dir build-person4-ninja --output-on-failure
```

Sau đó thử build production target:

```powershell
cmake --build build-person4 --config Debug --target SuperMario
```

Ghi vào note cá nhân:

```text
BASELINE
Commit: <git rev-parse --short HEAD>
Platform/compiler: <MSVC hoặc MinGW/GCC>
Configure: PASS/FAIL
Build Person2RuntimeContracts: PASS/FAIL
CTest: PASS/FAIL, số test
Build SuperMario: PASS/FAIL
Lỗi môi trường nếu có: <ghi nguyên lỗi đầu tiên>
```

Nếu baseline fail trước khi Nhân sửa:

- Không sửa đại source P1/P2/P3.
- Lưu command và error đầu tiên.
- Kiểm tra lỗi là source, dependency hay đường dẫn có dấu cách.
- Báo team bằng commit + command + error, không chỉ nói “build không được”.

---

## 6. Bước 3 — Chốt API tối thiểu trước khi code phần tích hợp

Nhân không cần đợi mọi người code xong mới bắt đầu. Nhân phải gửi sớm các yêu cầu contract sau.

### 6.1 Gửi P3: contract animation

```text
XREQ-CON-P3-P4-ANIMATION-001
Consumer task: P4-ANIMATION-001
Provider: Person 3
P4 cần P3 xác nhận:
1. Danh sách key semantic tối thiểu mà player/enemy dùng, ví dụ Idle/Run/Jump/Fall/Hurt/Dead.
2. P3 sở hữu sf::Sprite và sf::Texture; lifetime phải dài hơn AnimationComponent.
3. P3 register frame trước update và chỉ gọi play/update một lần mỗi entity mỗi frame.
4. Cho một consumer fixture tối thiểu: một player và một enemy để test chuyển key.
P4 sẽ cung cấp deterministic frame playback; P4 không chọn gameplay state hay atlas coordinate cho P3.
Acceptance: compile + visible switch không missing-key spam.
```

### 6.2 Gửi P1: contract GUI

```text
XREQ-CON-P1-P4-GUI-001
Consumer task: P4-GUI-001
Provider: Person 1
P4 cần P1 cung cấp danh sách action value tối thiểu cho:
- Main: Start, Settings, Leaderboard, Exit
- EnterName: Confirm, Cancel
- Select: SelectCharacter, SelectStage, Confirm, Back
- Death/Win: Retry/Next/BackToMenu
- Pause: Resume/BackToMenu
P1 sở hữu navigation/state replacement.
P4 chỉ render, focus, hit-test, text edit và trả về tối đa một action value cho một input edge.
Không truyền State* hoặc callback giữ sống lâu hơn state.
Acceptance: P1 tạo/own control và queue transition sau handler.
```

### 6.3 Gửi P1: audio event

```text
XREQ-CON-P1-P4-AUDIO-EVENTS-001
Consumer task: P4-AUDIO-001
Provider: Person 1
P4 cần typed cue/music request dạng value, tối thiểu gồm:
Jump, Coin, Mushroom, BrickBreak, Shoot, Button, Death, GameOver, Win,
MenuMusic, Level1Music, Level2Music, Level3Music, StopMusic.
P1 phát semantic event đúng một lần; P4 quản lý buffer/music/playback.
Thiếu file audio phải trả failure nhưng gameplay vẫn chạy.
```

### 6.4 Gửi P1: HUD snapshot

```text
XREQ-CON-P1-P4-HUD-DATA-001
Consumer task: P4-HUD-001
Provider: Person 1
Đề nghị P1 cung cấp immutable value snapshot mỗi frame:
playerName, score, coins, lives, health (nếu dùng), stageId, remainingTime.
P4 không giữ Player*, LevelManager* hoặc State*.
P1 update snapshot sau gameplay event và render HUD bằng UI view.
Acceptance: camera chạy nhưng HUD đứng yên trên màn hình.
```

### 6.5 Gửi P2: block context và variant

Quyết định đã duyệt:

- Player hit từ dưới kích hoạt payload block.
- Chỉ Big/Fire player phá Brick thường.
- Small player chỉ bump Brick.
- Enemy, boss và shell không kích hoạt/phá mặc định.

Mẫu gửi:

```text
XREQ-CON-P2-P4-BLOCK-COLLISION-001
Consumer task: P4-PAYLOAD-BLOCK-001
Provider: Person 2, đọc power state từ Person 3
P4 cần P2 truyền một BlockHitContext nhỏ hoặc equivalent gồm:
- collision side
- actor category/identity
- player form: Small/Big/Fire khi actor là player
- relative motion nếu caller cần xác nhận underside hit
P4 trả semantic result: None/Bounce/Empty/Break/SpawnRequested.
Unknown actor phải non-activating; solid collision vẫn an toàn.
P2 tiếp tục sở hữu collision resolution, same-frame filtering và runtime object.
```

```text
XREQ-CON-P2-P4-BLOCK-VARIANTS-001
Consumer task: P4-PAYLOAD-BLOCK-001
Provider: Person 2
P2 cần forward MapSpawnInfo.variant vào constructor/API P4 một lần lúc load.
P4 không đọc pixel map.
Supported: normal/coin Brick và Mushroom/1-Up/Star question payload theo API P3.
Unknown variant: diagnostic + reject/safe behavior, không âm thầm spawn sai reward.
CloudPlatform không nằm trong release hiện tại.
```

### 6.6 Gửi P3: payload item

```text
XREQ-CON-P3-P4-PAYLOAD-001
Consumer task: P4-PAYLOAD-BLOCK-001
Provider: Person 3
P4 cần constructor/factory ổn định cho Mushroom, 1-Up và Star:
- input position world và popped/emerging state
- output fresh heap object hoặc unique_ptr theo seam đã thống nhất với P2
- item tự sở hữu behavior/effect; P4 chỉ chọn variant và request spawn
P4 không implement item effect, movement hay collection.
Acceptance: mỗi qualifying block hit tạo đúng một payload, P2 adopt đúng một lần.
```

### 6.7 Quy tắc khi API chưa trả lời

- Làm phần core/headless không phụ thuộc provider trước.
- Tạo test fake/value fixture trong file test của P4, không tạo production mock framework.
- Không include private header của người khác để “đi đường vòng”.
- Không đoán raw pointer ownership.
- Ghi task `BLOCKED_BY_OTHER_TASK`, nêu đúng symbol/file đang chờ.

---

## 7. Bước 4 — Làm `P4-ANIMATION-001`

### 7.1 File được sửa

- `include/Components/AnimationComponent.hpp`
- `src/Components/AnimationComponent.cpp`
- `tests/Person4AnimationContracts.cpp`

P3 entity files chỉ đọc, không sửa.

### 7.2 Contract tối thiểu cần đạt

1. `AnimationComponent` chỉ mượn `sf::Sprite` và `sf::Texture`; owner phải sống lâu hơn component.
2. Không cho copy nếu copy làm reference alias không an toàn.
3. Interval phải lớn hơn 0.
4. Animation rỗng bị reject tại lúc register.
5. Duplicate key có kết quả rõ ràng, không catch rồi chỉ in log.
6. Missing key không crash, không đổi sang frame rác và không spam mỗi frame.
7. Đổi key phải set frame 0 ngay và reset timer.
8. `dt` lớn phải giữ thời gian dư và advance deterministic.
9. Không thêm animation graph, transition tree hoặc dependency mới.

Một API nhỏ có thể dùng `bool` cho `addAnimation`/`play`, hoặc enum result nhỏ nếu thật sự cần phân biệt failure. Chọn một cách và viết test; không tạo cả exception lẫn boolean lẫn log cho cùng một lỗi.

### 7.3 Test bắt buộc

- Register key hợp lệ.
- Duplicate key.
- Empty frames.
- Missing key.
- Interval bằng 0 và âm.
- Key mới áp frame 0 ngay.
- `dt` đúng bằng interval.
- `dt` bằng nhiều interval cộng phần dư.
- Chuyển A → B → A.
- Texture/sprite vẫn do caller sở hữu.

### 7.4 Commit

```powershell
git add include/Components/AnimationComponent.hpp src/Components/AnimationComponent.cpp tests/Person4AnimationContracts.cpp
git diff --cached --check
git commit -m 'person4: stabilize animation playback contract'
git push
```

Không chuyển card sang `DONE` cho đến khi P3 có consumer compile/runtime/visual evidence. Sau source test, status hợp lý là `TESTING` hoặc ghi rõ `SOURCE_DONE=1`, các dimension còn lại chưa xong.

---

## 8. Bước 5 — Làm `P4-BLOCK-SIZE-001`

### 8.1 Root cause

Atlas frame thường là 16×16 nhưng P2 đặt world size 32×32 bằng `object->setSize(mapSize)`. `Block` hiện không override đường gọi đó; `setSizeBlock()` là một API thứ hai mà P2 không gọi. Vì vậy có thể collision 32×32 nhưng sprite chỉ 16×16.

### 8.2 Cách sửa tối thiểu

- Giữ `GameObject::size/hitbox` là source of truth.
- Cho virtual `setSize(const sf::Vector2f&)` mà P2 đang gọi đi qua logic scale của `Block`.
- Scale dựa trên texture rect hiện hành, không hard-code theo từng subclass.
- Khi frame animation đổi, visual footprint vẫn bằng world size.
- Rect rỗng/zero không được chia cho 0; phải có policy testable.
- Repeated `setSize(32,32)` không double-scale.
- Không sửa call site trong `LevelManager` chỉ để gọi `setSizeBlock()`.

Nếu giữ `setSizeBlock()` vì tương thích, nó nên reuse đúng một implementation; không duy trì hai công thức scale.

### 8.3 File được sửa

- `include/Objects/Blocks/Block.hpp`
- `src/Objects/Blocks/Block.cpp`
- Chỉ sửa subclass P4 nếu initialization order bắt buộc.
- `tests/Person4BlockContracts.cpp`

### 8.4 Test bắt buộc

- Frame 16×16 + world 32×32 → rendered bounds 32×32.
- Repeated set size.
- Đổi animation frame cùng kích thước.
- Rect chưa có rồi mới init.
- Fixture 32×64.
- Hitbox và visible bounds cùng position/size.
- SolidBlock, Brick, CoinBlock, MushroomBlock.
- Rerun Brick/Fragment/lifecycle regression của P2.

### 8.5 Commit

```powershell
git add include/Objects/Blocks/Block.hpp src/Objects/Blocks/Block.cpp tests/Person4BlockContracts.cpp
git diff --cached --check
git commit -m 'person4: keep block sprites aligned with world size'
git push
```

---

## 9. Bước 6 — Làm `P4-GUI-001`

### 9.1 Mục tiêu phiên bản đầu

Chỉ xây bộ control tối thiểu để P1 làm được selection/death/win slice:

- Một button/selectable control.
- Normal/focused/hover/pressed/disabled state.
- Keyboard focus next/previous và activate.
- Mouse hit test.
- Text input cho EnterName: nhập, backspace, confirm, cancel và giới hạn độ dài.
- Resize/layout cơ bản.
- Trả action dạng value cho P1.

Không xây UI framework tổng quát, scene graph, theme engine hoặc state machine mới.

### 9.2 Ownership

- P1 state tạo và sở hữu control/container trong lifetime của state.
- P4 control mượn font/texture có lifetime ổn định hoặc dùng resource owner đã thống nhất.
- P4 không gọi `changeState`, `pushState`, `popState`.
- Một key/mouse edge chỉ phát tối đa một activation.

### 9.3 Test bắt buộc

- Focus traversal thuận/ngược.
- Enter/Space activate đúng một lần.
- Disabled không activate.
- Mouse inside/outside bounds.
- Focus loss không kích hoạt stale control.
- Text entry, Unicode policy, backspace, max length, confirm/cancel.
- Resize không làm text tràn control.
- Missing font/resource có error ổn định.
- Visual check 800×600 và một kích thước resize.

### 9.4 Commit

```powershell
git add include/UI/GUI.hpp src/UI/GUI.cpp
git add tests/Person4GUIContracts.cpp
git diff --cached --check
git commit -m 'person4: add reusable accessible menu controls'
git push
```

Chỉ thêm `tests/Person4GUIContracts.cpp` nếu thực sự tạo file này; không copy nguyên test command nếu file không tồn tại.

---

## 10. Bước 7 — Làm `P4-QUESTION-BLOCK-001`

### 10.1 Những behavior phải giữ

- Coin spawn tại `{block.x, block.y - 32}` với `popped=true`.
- Mushroom spawn tại `{block.x, block.y - 32}` với popped/emerging state hiện có.
- Callback raw pointer hiện là **transfer-only**: P2 phải wrap thành một `unique_ptr` đúng một lần ngay khi callback.
- P4 không giữ pointer sau callback.
- Empty block không spawn lần nữa.

### 10.2 Việc cần sửa/test

- Modernize `tests/TestCoinBlock.cpp` sang SFML 3.1.
- Ghi nhận original Y ổn định, không phụ thuộc một update vô tình chạy trước/sau `setPosition`.
- Bounce hữu hạn và snap đúng origin.
- Hit lặp trong lúc bounce không tạo payload ngoài contract.
- `CoinBlock(N)` spawn đúng N lần rồi Empty.
- `CoinBlock(0)` hoặc số âm có policy rõ ràng.
- Null callback không crash/leak.
- Mushroom chỉ spawn một lần.
- Active/Empty animation đúng và sprite 32×32.
- P2 adopt pending object sau traversal, không double update trong cùng lượt.

Không làm 1-Up/Star ở card này; đó là payload variant card.

### 10.3 Commit

```powershell
git add include/Objects/Blocks/CoinBlock.hpp src/Objects/Blocks/CoinBlock.cpp
git add include/Objects/Blocks/MushroomBlock.hpp src/Objects/Blocks/MushroomBlock.cpp
git add tests/TestCoinBlock.cpp tests/TestMushroomBlock.cpp tests/Person4BlockContracts.cpp
git diff --cached --check
git commit -m 'person4: validate coin and mushroom block behavior'
git push
```

Chỉ stage những test file thực tế đã sửa/tạo.

---

## 11. Bước 8 — Làm `P4-AUDIO-001`

### 11.1 API tối thiểu

- Typed `SoundCue` và `MusicTrack`, không truyền string path từ gameplay mỗi lần.
- Load/cache `sf::SoundBuffer` để buffer sống lâu hơn `sf::Sound`.
- Music được stream và có play/switch/stop.
- SFX có thể overlap khi hợp lý.
- Music/SFX volume tách riêng, clamp trong khoảng hợp lệ.
- Mute/unmute.
- Không restart cùng music mỗi frame.
- Missing/unsupported file trả kết quả lỗi, không crash gameplay.
- Cleanup deterministic.

PLAN có nhắc application-visible service/singleton policy, nhưng không copy manual singleton `new` từ Group5. Nếu app đã có một owner lâu dài thì dùng owner đó; chỉ dùng singleton khi P1 xác nhận lifecycle.

### 11.2 File được sửa

- `include/Audio/AudioSystem.hpp`
- `src/Audio/AudioSystem.cpp`
- Test P4 phù hợp.

### 11.3 Test bắt buộc

- Load/play/stop một music.
- Switch music và lặp request cùng track.
- Play hai cue gần nhau.
- Volume 0, max, dưới min, trên max.
- Mute/unmute.
- Missing file.
- Destroy AudioSystem khi sound/music đang hoặc vừa phát.
- Sau integration: jump/coin/brick/death/win phát đúng một lần ở semantic boundary.

### 11.4 Commit

```powershell
git add include/Audio/AudioSystem.hpp src/Audio/AudioSystem.cpp
git diff --cached --check
git commit -m 'person4: add SFML 3 audio service and cue controls'
git push
```

---

## 12. Bước 9 — Làm phần progression in-memory trước

`P4-PERSISTENCE-001` chưa được đánh `DONE`, nhưng provider in-memory cho đúng chín stage có thể và nên làm sớm để gỡ block P1 selection/win.

### 12.1 Danh sách stage duy nhất

```text
W1_LV1 -> W1_LV2 -> W1_LV3 ->
W2_LV1 -> W2_LV2 -> W2_LV3 ->
W3_LV1 -> W3_LV2 -> W3_LV3 -> FINAL
```

### 12.2 Yêu cầu

- Represent stage bằng value/ID, không bằng State pointer.
- Validate world 1..3 và level 1..3.
- Default/current/unlocked snapshot hợp lệ.
- Có đúng tám successor và terminal `W3_LV3`.
- Completion trùng lặp không unlock/reward hai lần.
- Không unlock ngoài chín stage.
- API đọc đủ cho P1 Select/Win.
- Chưa cần file I/O trong commit này.

### 12.3 Test

- Tất cả chín ID hợp lệ.
- Những ID ngoài bảng bị reject.
- Tám transition liên tiếp.
- `W3_LV3` trả final, không sinh W4 hoặc LV4.
- Duplicate completion idempotent.
- Current không vượt unlocked.

### 12.4 Commit

```powershell
git add include/Core/UserData.hpp src/Core/UserData.cpp tests/Person4DataContracts.cpp
git diff --cached --check
git commit -m 'person4: validate nine-stage in-memory progression'
git push
```

Sau commit này, báo P1 rằng provider checklist đã sẵn sàng; không tự sửa selection/win state.

---

## 13. Bước 10 — Chờ provider rồi làm `P4-PAYLOAD-BLOCK-001`

Chỉ chuyển task sang `IN_PROGRESS` khi:

- P2 đã có/đã chấp nhận `BlockHitContext` equivalent.
- P2 forward map variant.
- P3 có API/implementation Mushroom, 1-Up và Star cần thiết.
- Normal Brick baseline test vẫn xanh.

### 13.1 P4 phải làm

- Định nghĩa enum/value block payload nhỏ.
- Brick mặc định giữ behavior hiện tại.
- Coin Brick tạo đúng payload đã duyệt.
- Mushroom/1-Up/Star question block chọn đúng payload.
- Unknown variant fail an toàn, không spawn nhầm Mushroom.
- Actor eligibility theo decision.
- Exactly-once spawn.
- Active/Empty/Inactive lifecycle rõ ràng.
- P4 chỉ request object; P2 nhận ownership; P3 sở hữu item behavior.

### 13.2 Bảng test eligibility

| Actor/form | Question/payload | Brick thường |
|---|---|---|
| Player Small, hit dưới | Activate | Bump, không vỡ |
| Player Big, hit dưới | Activate | Break |
| Player Fire, hit dưới | Activate | Break |
| Player hit bên/trên | Không activate | Không break |
| Enemy | Không activate | Không break |
| Boss | Không activate | Không break |
| Shell | Không activate mặc định | Không break mặc định |
| Unknown | Không activate | Không break |

### 13.3 Commit

```powershell
git add include/Objects/Blocks/Brick.hpp src/Objects/Blocks/Brick.cpp
git add include/Objects/Blocks/MushroomBlock.hpp src/Objects/Blocks/MushroomBlock.cpp
git add tests/Person4BlockContracts.cpp
git diff --cached --check
git commit -m 'person4: add variant-aware payload block behavior'
git push
```

Sau đó yêu cầu P2 chạy variant wiring/adoption test và rerun frozen Brick/Fragment suite.

---

## 14. Bước 11 — Làm `P4-HUD-001` sau khi P1 giao snapshot

### 14.1 HUD chỉ nhận value snapshot

Snapshot tối thiểu:

- Player name.
- Score.
- Coins.
- Lives.
- Health nếu UI dùng.
- Stage ID.
- Remaining time.

`LevelGUI` không giữ:

- `PlayerManager*`.
- `LevelManager*`.
- `GameEventMediator*` để tự suy đoán gameplay.
- State pointer.

### 14.2 Render order

1. P1 update gameplay.
2. P1 xử lý semantic event/UserData.
3. P1 tạo snapshot.
4. P4 `LevelGUI::update(snapshot)`.
5. P1 render world camera.
6. P1 restore screen/UI view.
7. P4 render HUD.

### 14.3 Test

- Snapshot default/min/max.
- Score/coin/lives đổi đúng một lần.
- Stage ID hợp lệ.
- Resize.
- Camera di chuyển nhưng HUD không trôi theo world.
- Missing optional field hiển thị placeholder ổn định.
- Text không bị cắt trên nền sáng/tối.

Commit:

```powershell
git add include/UI/LevelGUI.hpp src/UI/LevelGUI.cpp
git commit -m 'person4: add screen-space gameplay HUD'
git push
```

---

## 15. Bước 12 — Hoàn thành durable persistence

Decision đã chốt:

- Một shared profile cho 1P.
- Text format theo dòng, có version.
- Lưu score/coins/lives, chín-level progression và settings.
- Dữ liệu ghi ở user-writable location, tách khỏi packaged read-only assets.
- Save sau level/death/settings transaction đã duyệt và clean exit; không save mỗi frame.
- Ghi file tạm, đóng/flush thành công rồi replace; giữ/recover last-good.

### 15.1 Thứ tự implementation

1. Chốt schema version 1 trong test.
2. Parse vào temporary value object.
3. Validate toàn bộ field.
4. Chỉ mutate live `UserData` sau khi parse + validate hoàn tất.
5. Missing file → default profile và result rõ ràng.
6. Corrupt/unknown version → không phá file cũ, báo diagnostic.
7. Save ra sibling temporary file.
8. Flush/close.
9. Replace file đích theo cách recoverable của platform.
10. Tích hợp event/save timing với P1.

Không thêm JSON library; standard library đủ cho format dòng nhỏ này.

### 15.2 Test

- Default/missing file.
- Round trip.
- Min/max counter.
- 99/100/199 coins.
- Lives zero.
- Tất cả chín stage.
- Name validation/Unicode policy.
- Corrupt, truncated và unknown version.
- Write fail nhưng file valid trước đó còn nguyên.
- Save sau event đúng một lần.

Commit:

```powershell
git add include/Core/UserData.hpp src/Core/UserData.cpp tests/Person4DataContracts.cpp
git add SaveData
git diff --cached --check
git commit -m 'person4: add validated profile persistence'
git push
```

Không commit save cá nhân hoặc file chứa tên/tiến độ test của một người dùng thật.

---

## 16. Bước 13 — Làm leaderboard

Chỉ bắt đầu khi GUI, persistence schema và P1 routing đã sẵn sàng.

### 16.1 Data requirements

- Schema/version rõ ràng.
- Missing/empty file không crash.
- Validate name và number.
- Tên có delimiter/quote được xử lý đúng theo format đã chọn.
- Sort score giảm dần.
- Tie-break deterministic và được document.
- Giới hạn số entry hiển thị/lưu.
- Corrupt row có policy rõ.
- Write failure giữ dữ liệu cũ.
- Submit completion đúng một lần.

### 16.2 State boundary

- P4 implement `LeaderboardState` và bảng UI.
- P1 route vào/ra state.
- P4 Back action trả intent hoặc dùng State API đã được P1 phê duyệt; không tự tạo một state stack khác.
- Reuse P4 GUI thay vì tạo button thứ hai bằng raw pointer.

### 16.3 Test

- Missing/empty file.
- Một/nhiều entry.
- Tie.
- Long name, delimiter và quote.
- Corrupt row.
- Entry limit.
- Write fail.
- Reopen app thấy submission đúng.
- Keyboard Back hoạt động.

Commit:

```powershell
git add include/States/Menus/LeaderboardState.hpp src/States/Menus/LeaderboardState.cpp
git add tests/Person4DataContracts.cpp
git commit -m 'person4: add persistent leaderboard state'
git push
```

---

## 17. Bước 14 — Không làm CloudBlock trong release này

`DEC-CLOUD-SEMANTICS` đã quyết định:

- CloudPlatform/CloudBlock nằm ngoài release hiện tại.
- Không implement hoặc wire `CloudBlock`.
- Không đổi marker đang có.
- Không sửa map pixel.
- `MovingBlock` vẫn thuộc P2 và độc lập.

Việc đúng của Nhân là ghi `P4-CLOUD-001 = DEFERRED/OUT_OF_CURRENT_SCOPE`, không phải cố làm nó để đủ số lượng class.

Nếu team muốn mở lại, phải reopen decision trước rồi mới viết code.

---

## 18. Bước 15 — Package/release làm cuối

### 18.1 Mục tiêu

- C++20 + SFML 3.1 build sạch.
- Một asset root cạnh executable.
- Copy đúng một cây `assets/` vào package.
- Font/audio/texture/map load không phụ thuộc repository CWD.
- Writable profile/leaderboard nằm ngoài read-only assets.
- Missing asset có diagnostic rõ.
- Chạy được từ package directory và một unrelated working directory.
- Có đủ file identity của chín map, không audit/edit pixel.

### 18.2 Không được làm

- Không thêm nhiều fallback `../`, `../../` ở từng loader.
- Không bundle save cá nhân.
- Không copy chatbot/Ollama/model của Group5.
- Không downgrade SFML.
- Không sửa gameplay để package “chạy”.

### 18.3 Kiểm tra release

```powershell
cmake -S . -B build-person4-release
cmake --build build-person4-release --config Release
ctest --test-dir build-person4-release -C Release --output-on-failure
```

Sau đó chạy executable từ package root và từ một working directory khác. Kiểm tra:

- Font/menu.
- Block/player/enemy texture.
- Music/SFX.
- Load một level.
- HUD.
- Save/reload.
- Leaderboard.
- Missing một asset trong package test phải fail an toàn và báo đúng file.

Commit:

```powershell
git add CMakeLists.txt
git add <các file package/install thực tế>
git diff --cached --check
git commit -m 'person4: package the SFML 3.1 production build'
git push
```

---

## 19. Có thể lấy gì từ Group5

Group5 chỉ là reference. Port behavior nhỏ, không port kiến trúc.

| Phần Group5 | Có thể lấy/tham khảo | Không được copy nguyên |
|---|---|---|
| `AnimationComponent.*` | Ý tưởng reset frame 0 và giữ phần thời gian dư | API SFML 2, frame chọn từ unordered-map prefix, update khi frame rỗng, raw ownership assumptions |
| `SolidBlock.cpp` | Atlas coordinates/ý tưởng scale sau khi có frame | Constructor, hard-coded `Source/Resources`, hitbox duplication |
| `CoinBlock.cpp`, `MushroomBlock.cpp` | Frame/bounce feel để so sánh trực quan | Direct mediator mutation, raw spawn, fixed paths, SFML 2, block tự tăng score/audio |
| `Brick.cpp`, `BrickFragment.cpp` | Visual rotation/spread feel nếu cần polish | Random 12–15 fragments, fragment nằm trong Brick, raw mediator/delete, thay baseline bốn fragment |
| `AudioSystem.*` | Danh mục cue và danh sách asset | Manual singleton `new`, hard-coded path, throw khi thiếu file, coupling `GameEventMediator`, SFML 2 constructors |
| `GUI.*` | Màu sắc, layout, hover/pressed style | Raw widget pointer, mouse-only behavior, Audio singleton coupling, State navigation |
| `LevelGUI.*` | Các field hiển thị và bố cục tham khảo | Giữ `PlayerManager*`, `LevelManager*`, mediator, tự suy đoán gameplay state |
| `UserData.*` | Ý tưởng chín score slot và file text/CSV | `vector<PlayerManager*>`, serialize live player, parser không validate, path cố định |
| `LeaderboardState.*` | Bố cục bảng, cột, ý tưởng sort/limit | `stoi` không guard, parser CSV không quote-safe, raw Back button, Group5 State ownership |
| Assets | Chỉ reuse file đã có license/provenance và hash đúng | Copy toàn bộ Resources, save cá nhân, chatbot/LLM assets |
| `CloudBlock.*` | Không dùng trong release này | Toàn bộ behavior, vì decision hiện tại đã defer CloudBlock |

### Quy trình port một đoạn từ Group5

1. Viết behavior cần đạt bằng một câu.
2. Tìm xem Group4 đã có helper/API tương đương chưa.
3. Chỉ mở file Group5 liên quan.
4. Copy tối đa thuật toán/frame/value cần thiết, không copy class hierarchy.
5. Đổi sang SFML 3.1 và ownership Group4.
6. Bỏ hard-coded path, mediator coupling và raw owner.
7. Viết test trước khi gọi là port xong.
8. Trong commit message/body ghi `reference: Group5 <file>` nếu có dùng logic đáng kể.

Hiện Group4 đã có hầu hết texture/audio/font tương ứng. Không copy asset trùng chỉ vì thấy Group5 có cùng tên.

---

## 20. Cách kiểm tra code sau mỗi task

Sau mỗi commit:

```powershell
git status --short
git show --stat --oneline HEAD
git diff HEAD^ --check
cmake --build build-person4 --config Debug
ctest --test-dir build-person4 -C Debug --output-on-failure
```

Nếu task có visual/audio:

- Automated test không thay thế visual/audible evidence.
- Ghi scene, action, expected, actual và commit.
- Nếu chỉ compile được, không ghi `RUNTIME_TESTED=1`.
- Nếu runtime headless pass, không tự ghi `VISUALLY_VERIFIED=1`.
- Nếu một demo riêng pass nhưng production flow chưa gọi, không ghi `GAMEPLAY_VERIFIED=1`.

Mỗi task theo dõi năm dimension:

```text
SOURCE_DONE
INTEGRATION_DONE
RUNTIME_TESTED
VISUALLY_VERIFIED
GAMEPLAY_VERIFIED
```

`DONE` chỉ khi các dimension áp dụng đều có evidence. Audio có thể ghi visual `n/a`, nhưng vẫn cần runtime/gameplay/audible evidence.

---

## 21. Cách commit và merge để không đụng code người khác

### 21.1 Một commit chỉ làm một task

Các commit đề xuất:

```text
person4: stabilize animation playback contract
person4: keep block sprites aligned with world size
person4: add reusable accessible menu controls
person4: validate coin and mushroom block behavior
person4: add SFML 3 audio service and cue controls
person4: validate nine-stage in-memory progression
person4: add variant-aware payload block behavior
person4: add screen-space gameplay HUD
person4: add validated profile persistence
person4: add persistent leaderboard state
person4: package the SFML 3.1 production build
```

### 21.2 Trước khi merge code mới của đồng đội

```powershell
git status --short --branch
git fetch origin
git merge --no-ff origin/person2/map-integration
```

Với P1/P3, dùng đúng branch mà người đó thông báo đã push/test. Không merge một branch prototype vào production chỉ vì có nhiều class hơn.

### 21.3 Xử lý conflict

1. Xác định owner của file.
2. File P4-owned: giữ baseline P4 + áp phần consumer contract nhỏ cần thiết.
3. File P1/P2/P3-owned: giữ owner version, rồi request API nếu thiếu.
4. Không chọn `ours`/`theirs` cho cả file khi chưa đọc diff.
5. Build/test ngay sau resolution.
6. Ghi conflict resolution trong commit body nếu thay đổi contract.

---

## 22. Cách cập nhật master board và execution plan

Khi bắt đầu task, đổi đúng row:

```text
READY -> IN_PROGRESS
```

Khi source + local test xong nhưng chờ consumer:

```text
IN_PROGRESS -> TESTING
SOURCE_DONE=1
RUNTIME_TESTED=<theo evidence>
INTEGRATION_DONE=0 nếu consumer chưa merge
```

Khi bị chờ API:

```text
Status: BLOCKED
Blocker: XREQ-...
Provider: P1/P2/P3
Exact file/symbol: ...
Acceptance: ...
```

Khi hoàn tất:

```text
Status: DONE
Commit: <sha>
Commands: <build/test commands>
Outcome: <pass count>
Visual/gameplay evidence: <scene/check>
```

Không chuyển toàn bộ P4 sang done chỉ vì các shell compile.

---

## 23. Mẫu báo cáo mỗi lần push

Nhân có thể copy mẫu sau vào nhóm:

```text
[P4 UPDATE]
Task: <P4-...>
Branch: person4
Commit: <sha>
Status: IN_PROGRESS / TESTING / DONE / BLOCKED

Đã làm:
- ...

API cung cấp/thay đổi:
- ...

Test:
- Command: ...
- Result: ...

Regression:
- Brick/Fragment/lifecycle: PASS/FAIL/N-A

Còn thiếu:
- ...

Đang chờ:
- XREQ-... từ Person ...

Nhờ consumer làm:
- merge/cherry-pick <sha>
- chạy <integration scenario>
```

---

## 24. Checklist cuối cùng cho Nhân

### Git và baseline

- [ ] Tạo `person4` từ `origin/person2/map-integration`.
- [ ] Push upstream `origin/person4`.
- [ ] Baseline build/test được ghi lại.
- [ ] Không mang thay đổi `main.cpp` hoặc P1/P2/P3-owned file vào commit P4.

### P0

- [ ] `P4-ANIMATION-001` deterministic và có P3 consumer evidence.
- [ ] `P4-BLOCK-SIZE-001` visible bounds khớp 32×32 physics.
- [ ] `P4-GUI-001` phục vụ selection/death/win, keyboard + mouse.
- [ ] Nine-stage in-memory progression provider sẵn sàng cho P1.

### P1

- [ ] Question block test SFML 3.1, finite bounce, exactly-once spawn.
- [ ] Audio resource lifetime, volume, failure và event integration.
- [ ] Payload variants sau khi P2/P3 giao API.
- [ ] HUD nhận immutable snapshot và đứng yên theo screen view.
- [ ] Persistence versioned, validated và recoverable.
- [ ] CloudBlock vẫn deferred, không code nhầm.

### P2/release

- [ ] Leaderboard parser/sort/tie/error/state integration.
- [ ] SFML 3.1 release build.
- [ ] Một executable-adjacent asset root.
- [ ] Writable data tách khỏi assets.
- [ ] Package chạy từ unrelated CWD.

### Regression và evidence

- [ ] Normal Brick vẫn vỡ một lần.
- [ ] Tạo đúng bốn BrickFragment.
- [ ] Fragment spawn/adopt/expire/remove vẫn pass.
- [ ] Coin popped và Mushroom Y-32 vẫn đúng.
- [ ] Inactive object không re-collide cùng frame.
- [ ] Không raw owning pointer bị giữ sau callback.
- [ ] Không hard-coded Group5 path.
- [ ] Không Group5 chatbot/LLM code.
- [ ] Mỗi task có commit, command, result và completion dimensions.
- [ ] Push branch sau từng checkpoint để đồng đội nhìn thấy.

---

## 25. Thứ tự ngắn gọn để Nhân làm ngay

Nếu Nhân chỉ cần danh sách hành động hôm nay:

1. Tạo branch `person4` từ `origin/person2/map-integration` và push.
2. Chạy baseline `Person2RuntimeContracts` và production build.
3. Gửi sáu XREQ ở Mục 6 cho P1/P2/P3.
4. Làm `P4-ANIMATION-001`, test và push.
5. Làm `P4-BLOCK-SIZE-001`, rerun Brick regression và push.
6. Làm GUI core phục vụ selection/death/win và push.
7. Modernize/test CoinBlock + MushroomBlock và push.
8. Làm AudioSystem core và push.
9. Làm nine-stage progression in memory để gỡ block P1.
10. Khi API đến: payload block → HUD → persistence.
11. Sau đó leaderboard.
12. Cuối cùng package/release.
13. Không làm CloudBlock trong release này.

Nguyên tắc cuối:

> Khi chưa biết người khác cần API gì, đừng đoán implementation của họ. Viết contract value nhỏ, gửi XREQ có input/output/ownership/timing/failure/test, rồi tiếp tục phần độc lập của P4. Port Group5 chỉ ở mức frame, layout, cue list hoặc thuật toán nhỏ; Group4 contract và ownership luôn là nguồn đúng.
