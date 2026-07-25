# Tài liệu Thiết kế Kiến trúc Đồ án Super Mario (SFML)

Sơ đồ kiến trúc dưới đây mô tả chi tiết toàn bộ các lớp (class), thuộc tính (attributes), phương thức (methods), và mối quan hệ (inheritance, composition, association) trong project game Super Mario. Các thành phần liên quan đến AI/Chatbot đã được loại bỏ hoàn toàn để phản ánh đúng cấu trúc cốt lõi của game.

Do hệ thống lớp trong project khá lớn, tài liệu này được chia thành các sơ đồ chuyên biệt để bạn có cái nhìn chi tiết và trực quan nhất.

---

## 1. Sơ đồ Quản lý Trạng thái Game (State Management)
Quản lý luồng hoạt động của ứng dụng bằng **State Pattern**. `MyApp` là lớp khởi chạy và chứa một Stack các `State`.

```mermaid
classDiagram
    direction TB
    class MyApp {
        -sf::RenderWindow* window
        -sf::Event sfEvent
        -AudioSystem* audio
        -sf::Clock dtClock
        -float dt
        -StateData stateData
        -stack~State*~ states
        -initVariables() void
        -initWindow() void
        -initStateData() void
        -initStates() void
        -initAudioSystem() void
        +run() void
        +update() void
        +updateDT() void
        +updateSFMLEvents() void
        +render() void
    }

    class StateData {
        +sf::RenderWindow* window
        +stack~State*~* states
        +AudioSystem* audio
        +UserData* userData
    }

    class State {
        <<abstract>>
        #StateData* stateData
        #sf::RenderWindow* window
        #stack~State*~* states
        #sf::Vector2i mousePosWindow
        #bool quit
        +checkForQuit() void
        +endState() void
        +updateMousePosition() void
        +update(dt: float, event: sf::Event)* void
        +render(target: sf::RenderTarget*)* void
    }

    class MainMenuState {
        -sf::RectangleShape background
        -sf::Font font
        -vector~GUI::Button*~ buttons
        +update(dt: float, event: sf::Event) void
        +render(target: sf::RenderTarget*) void
    }

    class SettingState {
        -sf::RectangleShape background
        -sf::Font font
        -vector~GUI::Button*~ buttons
        -State* prevState
        +update(dt: float, event: sf::Event) void
        +render(target: sf::RenderTarget*) void
    }

    class LeaderboardState {
        -sf::RectangleShape background
        -sf::Font font
        -vector~GUI::Button*~ buttons
        +update(dt: float, event: sf::Event) void
        +render(target: sf::RenderTarget*) void
    }

    class GameState {
        -LevelManager* levelManager
        -PlayerManager* clonePlayer
        -int worldID
        -int levelID
        +reloadLevel() void
        +saveGame() void
        +checkPause() void
        +checkDeath() void
        +checkWin() void
        +update(dt: float, event: sf::Event) void
        +render(target: sf::RenderTarget*) void
    }

    class PauseMenuState
    class DeathMenuState
    class WinMenuState
    class MenuLevelState
    class MenuWorldState
    class MenuCharacterSelectionState

    MyApp *-- StateData : Chứa dữ liệu dùng chung
    MyApp *-- State : Quản lý ngăn xếp các State
    StateData o-- State : Tham chiếu đến danh sách State
    
    State <|-- MainMenuState
    State <|-- SettingState
    State <|-- LeaderboardState
    State <|-- GameState
    State <|-- PauseMenuState
    State <|-- DeathMenuState
    State <|-- WinMenuState
    State <|-- MenuLevelState
    State <|-- MenuWorldState
    State <|-- MenuCharacterSelectionState
```

---

## 2. Sơ đồ Lớp Các Đối Tượng Trong Game (GameObject Hierarchy)
Tất cả các thực thể hiển thị và tương tác vật lý trong game đều kế thừa từ `GameObject`.

```mermaid
classDiagram
    direction BT
    class GameObject {
        <<abstract>>
        #sf::Vector2f position
        #sf::Vector2f size
        #sf::Sprite entitySprite
        #sf::Texture entityTexture
        #GameEventMediator* eventMediator
        +sf::RectangleShape hitbox
        +getPosition() sf::Vector2f
        +getSize() sf::Vector2f
        +getCenter() sf::Vector2f
        +setPosition(pos: sf::Vector2f) void
        +setEventMediator(mediator: GameEventMediator*) void
        +update(dt: float)* void
        +render(target: sf::RenderTarget*)* void
    }

    %% ================= HỆ THỐNG BLOCK =================
    class Block {
        <<abstract>>
        #string name
        #unordered_map~string, sf::IntRect~ spritesSheet
        #bool exist
        +animationComponent: AnimationComponent*
        +initSpritesSheet()* void
        +reactToCollison(collidedSide: int)* void
    }
    class Brick
    class CoinBlock
    class CloudBlock
    class SolidBlock
    class MovingBlock
    class Pipe
    class Trampoline

    GameObject <|-- Block
    Block <|-- Brick
    Block <|-- CoinBlock
    Block <|-- CloudBlock
    Block <|-- SolidBlock
    Block <|-- MovingBlock
    Block <|-- Pipe
    Block <|-- Trampoline

    %% ================= HỆ THỐNG VẬT PHẨM (POWERUP) =================
    class PowerUpObject {
        <<abstract>>
        #string name
        #unordered_map~string, sf::IntRect~ spritesSheet
        +reactToCollison()* void
        +reactToEnemyCollision(enemy: Enemy*) void
        +reactToBlockCollision(block: Block*) void
    }
    class Mushroom
    class Coin
    class WinFlag
    class Lava
    class FireBuff

    GameObject <|-- PowerUpObject
    PowerUpObject <|-- Mushroom
    PowerUpObject <|-- Coin
    PowerUpObject <|-- WinFlag
    PowerUpObject <|-- Lava
    PowerUpObject <|-- FireBuff

    %% ================= HỆ THỐNG SINH VẬT (LIVING ENTITY) =================
    class LivingEntity {
        <<abstract>>
        #int health
        #int speed
        +animationComponent: AnimationComponent*
        +movementComponent: MovementComponent*
        +updateVelocity(dt: float) void
        +move(dt: float) void
        +updateAnimation(dt: float)* void
    }

    GameObject <|-- LivingEntity
```

---

## 3. Hệ thống Người chơi và Kẻ địch (Player & Enemies)
Kế thừa từ `LivingEntity` để sử dụng các cơ chế di chuyển, gia tốc, trọng lực và hoạt ảnh phức tạp.

```mermaid
classDiagram
    direction BT
    class LivingEntity {
        <<abstract>>
        #int health
        #int speed
        +animationComponent: AnimationComponent*
        +movementComponent: MovementComponent*
        +move(dt: float) void
        +updateVelocity(dt: float) void
    }

    %% ================= PLAYER =================
    class PlayerManager {
        #string m_imagePath
        #bool is_big
        #bool buffed_fire
        #bool is_fire
        #vector~PlayerBuff*~ buffs
        #float transformationTimer
        #bool isTransforming
        #string m_name
        #string currentAction
        +immortal: bool
        +isFlashing: bool
        +init() void
        +handleInput(dt: float) void
        +addBuff(buff: PlayerBuff*) void
        +removeBuff(type: string) void
        +updateHitboxSize() void
    }
    class Mario
    class Luigi
    
    LivingEntity <|-- PlayerManager
    PlayerManager <|-- Mario
    PlayerManager <|-- Luigi

    %% ================= ENEMY =================
    class Enemy {
        <<abstract>>
        +getIsAlive()* bool
        +setIsAlive(alive: bool)* void
        +getDamaged()* void
        +reactToPlayerCollision(collidedSide: int)* void
        +reactToBlockCollision(collidedSide: int)* void
    }
    class Goomba
    class Koopa
    class Bowser
    class FlyingKoopa
    class Heriss
    class PeteyPiranha
    class Rocket
    class Cannon

    LivingEntity <|-- Enemy
    Enemy <|-- Goomba
    Enemy <|-- Koopa
    Enemy <|-- Bowser
    Enemy <|-- FlyingKoopa
    Enemy <|-- Heriss
    Enemy <|-- PeteyPiranha
    Enemy <|-- Rocket
    Enemy <|-- Cannon
```

---

## 4. Cơ chế Hoạt động của Màn chơi (Level & Core Engine)
`LevelManager` đóng vai trò là "Controller" của từng màn chơi, điều phối các thành phần hỗ trợ bao gồm Renderer, Collision, Audio và Event Dispatching.

```mermaid
classDiagram
    class LevelManager {
        #MapManager* mapManager
        #sf::RenderWindow* window
        #PlayerManager* player
        #vector~Enemy*~ Enemies
        #vector~Block*~ Blocks
        #vector~PowerUpObject*~ PowerUps
        #LevelGUI* levelGUI
        #GameEventMediator* eventMediator
        #PhysicsEngine* physicsEngine
        #AudioSystem* audio
        #bool* finishedLevel
        +initGameEventMediator() void
        +update(dt: float) void
        +render(target: sf::RenderTarget*) void
        +getScore() int
    }

    class W1_LV1
    class W1_LV2
    class W1_LV3
    class W2_LV1
    class W2_LV2
    class W2_LV3
    
    LevelManager <|-- W1_LV1
    LevelManager <|-- W1_LV2
    LevelManager <|-- W1_LV3
    LevelManager <|-- W2_LV1
    LevelManager <|-- W2_LV2
    LevelManager <|-- W2_LV3

    class MapManager {
        -sf::RenderWindow* window
        +update(player: PlayerManager*, dt: float) void
        +draw_map(target: sf::RenderTarget*) void
    }

    class LevelGUI {
        -sf::Text scoreText
        -sf::Text coinsText
        -sf::Text worldText
        -sf::Text timeText
        +update(dt: float) void
        +render(target: sf::RenderTarget*) void
    }

    class PhysicsEngine {
        +checkCollision(obj1: GameObject*, obj2: GameObject*) int
        +resolveCollisions(player: PlayerManager*, blocks: vector~Block*~, enemies: vector~Enemy*~) void
    }

    class GameEventMediator {
        -PlayerManager* player
        -vector~Enemy*~* enemies
        -vector~Block*~* blocks
        -PhysicsEngine* physicsEngine
        -LevelGUI* levelGUI
        -AudioSystem* audio
        +updateInput(dt: float) void
        +updateEvents(dt: float) void
    }

    class AudioSystem {
        <<Singleton>>
        -static AudioSystem* instance
        -unordered_map~string, sf::SoundBuffer~ soundBuffers
        -sf::Music backgroundMusic
        +getInstance() static AudioSystem*
        +playSound(name: string) void
        +playMusic(name: string) void
    }

    LevelManager *-- MapManager
    LevelManager *-- LevelGUI
    LevelManager *-- PhysicsEngine
    LevelManager *-- GameEventMediator
    LevelManager o-- AudioSystem
    GameEventMediator o-- PhysicsEngine
    GameEventMediator o-- AudioSystem
    GameEventMediator o-- LevelGUI
```

---

## 5. Mối quan hệ giữa các Thành phần Hỗ trợ (Components)
Mỗi thực thể `LivingEntity` đều sở hữu `AnimationComponent` và `MovementComponent` để tách biệt phần xử lý logic di chuyển và vẽ hoạt ảnh khỏi logic lớp chính.

```mermaid
classDiagram
    class LivingEntity {
        +AnimationComponent* animationComponent
        +MovementComponent* movementComponent
    }

    class AnimationComponent {
        -sf::Sprite& sprite
        -sf::Texture& textureSheet
        -float animationTimer
        -float animationInterval
        -vector~sf::IntRect~ frames
        -int currentFrame
        +addAnimation(key: string, frames: vector~sf::IntRect~) void
        +play(key: string, dt: float) void
    }

    class MovementComponent {
        -sf::Vector2f velocity
        -sf::Vector2f maxVelocity
        -sf::Vector2f deceleration
        -sf::Vector2f acceleration
        +update(dt: float) void
        +accelerate(x: float, y: float) void
        +decelerate() void
    }

    LivingEntity *-- AnimationComponent
    LivingEntity *-- MovementComponent
```

---

## Tóm tắt các Design Pattern được áp dụng trong Đồ án:
1. **State Pattern**: Dùng để quản lý các trạng thái/màn hình của game thông qua class `State` và ngăn xếp `states` trong `MyApp`.
2. **Mediator Pattern**: Thể hiện qua `GameEventMediator`, giúp làm cầu nối trao đổi sự kiện và giải quyết va chạm giữa các thực thể (`Player`, `Enemies`, `Blocks`, `AudioSystem`, `LevelGUI`) mà không bắt chúng phải tham chiếu trực tiếp chéo nhau.
3. **Singleton Pattern**: Được áp dụng vào `AudioSystem` để đảm bảo tài nguyên âm thanh chỉ được tải một lần duy nhất và có thể truy cập từ bất kỳ đâu.
4. **Factory Method Pattern**: Class `Character` với hàm static `createPlayer` giúp sinh đối tượng `PlayerManager` (chọn Mario hoặc Luigi) dựa trên chuỗi tên truyền vào.
5. **Component Pattern**: Class `LivingEntity` chia tách các chức năng thành `AnimationComponent` và `MovementComponent` để tăng khả năng tái sử dụng.
