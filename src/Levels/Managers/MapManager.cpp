#include "Levels/Managers/MapManager.hpp"
#include "Core/AssetLocator.hpp"
#include <algorithm>
#include <array>
#include <iostream>
#include <sstream>

namespace {
namespace Palette {
constexpr std::array<sf::Color, 17> SOLID_GROUND{{
    {255, 200, 200}, {220, 220, 220}, {240, 240, 240}, {255, 225, 225},
    {200, 200, 200}, {255, 175, 175}, {117, 117, 117}, {89, 89, 89},
    {180, 180, 180}, {255, 150, 150}, {74, 134, 232}, {0, 255, 255},
    {0, 0, 255}, {201, 218, 248}, {208, 224, 227}, {207, 226, 243},
    {193, 113, 52}
}};
constexpr sf::Color BRICK{146, 73, 0};
constexpr sf::Color BREAKABLE_BRICK{81, 34, 19};
constexpr sf::Color COIN_BLOCK{255, 146, 85};
constexpr sf::Color MUSHROOM_BLOCK{246, 109, 109};
constexpr sf::Color GROW_MUSHROOM_BLOCK{146, 73, 100};
constexpr sf::Color DOUBLE_HEAL_MUSHROOM_BLOCK{146, 73, 150};
constexpr sf::Color LAVA{255, 77, 0};
constexpr sf::Color LAVA_BOTTOM{175, 55, 0};
constexpr sf::Color TRAMPOLINE{198, 60, 60};
constexpr sf::Color CANNON_MID{67, 67, 67};
constexpr sf::Color CANNON_BOTTOM{102, 102, 102};
constexpr sf::Color COIN{255, 177, 13};
constexpr sf::Color PLAYER_1{255, 0, 0};
constexpr sf::Color PLAYER_2{0, 255, 0};
constexpr sf::Color GOOMBA{155, 0, 0};
constexpr sf::Color KOOPA{0, 125, 0};
constexpr sf::Color FLYING_KOOPA{131, 162, 164};
constexpr sf::Color HERISS{179, 133, 186};
constexpr sf::Color PETEY_PIRANHA{13, 162, 164};
constexpr sf::Color BOWSER{128, 0, 128};
}

bool sameRgb(const sf::Color& lhs, const sf::Color& rhs) {
    return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b;
}

bool isEmptyMapPixel(const sf::Color& color) {
    return color.a < 128;
}

sf::Vector2f toWorldPosition(unsigned int x, unsigned int localY) {
    return {static_cast<float>(x) * MapFormat::TILE_SIZE,
            static_cast<float>(localY) * MapFormat::TILE_SIZE};
}
}

MapManager::MapManager(float gridSize)
    : gridSize(gridSize <= 0.f ? MapFormat::TILE_SIZE : gridSize),
      spawnPointP1{0.f, 0.f}, spawnPointP2{0.f, 0.f} {}

bool MapManager::loadFromImage(const std::string& filename) {
    sf::Image image;
    const auto resolved = AssetLocator::find(filename);
    if (!resolved || !image.loadFromFile(*resolved)) {
        std::cerr << "MapManager: Failed to load image " << filename << std::endl;
        return false;
    }

    // Xóa toàn bộ dữ liệu của màn chơi trước đó
    solidBlocks.clear();
    goombaSpawns.clear();
    coinBlockSpawns.clear();
    mushroomBlockSpawns.clear();
    brickSpawns.clear();
    spawnPointP1 = {0.f, 0.f};
    spawnPointP2 = {0.f, 0.f};

    // Kiểm tra kích thước ảnh quá lớn
    if (image.getSize().x > 1000 || image.getSize().y > 1000) {
        std::cerr << "MapManager: Image size is too large: " << image.getSize().x << "x" << image.getSize().y << std::endl;
        return false;
    }

    // Bộ lọc màu sắc bỏ qua kênh Alpha (để so sánh chính xác hơn)
    auto matchColor = [](const sf::Color& c1, const sf::Color& c2) {
        return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b;
    };

    for (unsigned int y = 0; y < image.getSize().y; ++y) {
        for (unsigned int x = 0; x < image.getSize().x; ++x) {
            sf::Color pixel = image.getPixel(sf::Vector2u(x, y));

            // Bỏ qua các pixel quá trong suốt (không hiển thị)
            if (pixel.a < 128) continue;

            // 1. Màu đen (0, 0, 0) đại diện cho khối đất cứng
            if (matchColor(pixel, sf::Color::Black)) {
                sf::FloatRect block(sf::Vector2f(x * gridSize, y * gridSize), sf::Vector2f(gridSize, gridSize));
                solidBlocks.push_back(block);
            }
            // 2. Màu đỏ (255, 0, 0) đại diện cho điểm spawn Player 1
            else if (matchColor(pixel, sf::Color::Red)) {
                spawnPointP1 = sf::Vector2f(x * gridSize, y * gridSize);
            }
            // 3. Màu xanh lá (0, 255, 0) đại diện cho điểm spawn Player 2
            else if (matchColor(pixel, sf::Color::Green)) {
                spawnPointP2 = sf::Vector2f(x * gridSize, y * gridSize);
            }
            // 4. Màu xanh dương (0, 0, 255) đại diện cho điểm sinh Goomba
            else if (matchColor(pixel, sf::Color::Blue)) {
                goombaSpawns.push_back(sf::Vector2f(x * gridSize, y * gridSize));
            }
            // 5. Màu vàng (255, 255, 0) đại diện cho khối quà chứa xu (CoinBlock)
            else if (matchColor(pixel, sf::Color::Yellow)) {
                coinBlockSpawns.push_back(sf::Vector2f(x * gridSize, y * gridSize));
            }
            // 6. Màu hồng cánh sen (255, 0, 255) đại diện cho khối quà chứa nấm (MushroomBlock)
            else if (matchColor(pixel, sf::Color::Magenta)) {
                mushroomBlockSpawns.push_back(sf::Vector2f(x * gridSize, y * gridSize));
            }
            // 7. Màu nâu (128, 64, 0) đại diện cho gạch thường có thể phá vỡ (Brick)
            else if (matchColor(pixel, sf::Color(128, 64, 0))) {
                brickSpawns.push_back(sf::Vector2f(x * gridSize, y * gridSize));
            }
        }
    }

    return true;
}

void MapManager::drawDebug(sf::RenderWindow& window) const {
    // Vẽ viền đỏ cho các khối hitbox tĩnh
    for (const auto& block : solidBlocks) {
        sf::RectangleShape rect(sf::Vector2f(block.size.x, block.size.y));
        rect.setPosition(sf::Vector2f(block.position.x, block.position.y));
        rect.setFillColor(sf::Color::Transparent);
        rect.setOutlineColor(sf::Color::Red);
        rect.setOutlineThickness(1.f);
        window.draw(rect);
    }
}

const std::vector<sf::FloatRect>& MapManager::getSolidBlocks() const {
    return solidBlocks;
}

std::vector<sf::FloatRect> MapManager::getNearbyBlocks(const sf::FloatRect& entityHitbox, float radius) const {
    std::vector<sf::FloatRect> nearbyBlocks;
    for (const auto& block : solidBlocks) {
        if (std::abs(block.position.x - entityHitbox.position.x) < radius) {
            nearbyBlocks.push_back(block);
        }
    }
    return nearbyBlocks;
}

const sf::Vector2f& MapManager::getSpawnPoint() const {
    return spawnPointP1;
}

const sf::Vector2f& MapManager::getSpawnPointP1() const {
    return spawnPointP1;
}

const sf::Vector2f& MapManager::getSpawnPointP2() const {
    return spawnPointP2;
}

const std::vector<sf::Vector2f>& MapManager::getGoombaSpawns() const {
    return goombaSpawns;
}

const std::vector<sf::Vector2f>& MapManager::getCoinBlockSpawns() const {
    return coinBlockSpawns;
}

const std::vector<sf::Vector2f>& MapManager::getMushroomBlockSpawns() const {
    return mushroomBlockSpawns;
}

const std::vector<sf::Vector2f>& MapManager::getBrickSpawns() const {
    return brickSpawns;
}

bool MapManager::loadMap(const std::string& filename) {
    const auto resolved = AssetLocator::find(filename);
    currentFile = resolved ? resolved->string() : filename;
    lastError.clear();
    mapWidth = 0;
    spawns.clear();
    diagnostics.clear();

    sf::Image image;
    if (!resolved || !image.loadFromFile(*resolved)) {
        lastError = AssetLocator::missingMessage(filename);
        std::cerr << "MapManager: " << lastError << '\n';
        return false;
    }

    const sf::Vector2u size = image.getSize();
    if (size.x == 0) {
        lastError = "Map image has zero width: " + filename;
        std::cerr << "MapManager: " << lastError << '\n';
        return false;
    }
    if (size.y != MapFormat::REQUIRED_MAP_HEIGHT &&
        size.y != MapFormat::GROUP5_MAP_HEIGHT) {
        std::ostringstream message;
        message << "Invalid map height for " << filename << ": expected "
                << MapFormat::REQUIRED_MAP_HEIGHT << " or "
                << MapFormat::GROUP5_MAP_HEIGHT << ", got " << size.y;
        lastError = message.str();
        std::cerr << "MapManager: " << lastError << '\n';
        return false;
    }

    mapWidth = size.x;
    for (unsigned int x = 0; x < mapWidth; ++x) {
        for (unsigned int localY = 0;
             localY < MapFormat::LOGICAL_MAP_HEIGHT; ++localY) {
            decodeObjectPixel(image.getPixel({x, localY}), x, localY);
            decodeActorPixel(
                image.getPixel({x, localY + MapFormat::LOGICAL_MAP_HEIGHT}),
                x, localY);
        }
    }
    return true;
}

const std::vector<MapSpawnInfo>& MapManager::getSpawns() const noexcept {
    return spawns;
}

const std::vector<MapDiagnostic>& MapManager::getDiagnostics() const noexcept {
    return diagnostics;
}

unsigned int MapManager::getMapWidth() const noexcept {
    return mapWidth;
}

std::optional<sf::FloatRect> MapManager::getWorldBounds() const noexcept {
    if (mapWidth == 0) return std::nullopt;
    return sf::FloatRect(
        {0.f, 0.f},
        {static_cast<float>(mapWidth) * gridSize,
         static_cast<float>(MapFormat::LOGICAL_MAP_HEIGHT) * gridSize});
}

const std::string& MapManager::getLastError() const noexcept {
    return lastError;
}

void MapManager::decodeObjectPixel(const sf::Color& color, unsigned int x,
                                   unsigned int localY) {
    if (isEmptyMapPixel(color)) return;

    MapSpawnInfo spawn;
    spawn.position = toWorldPosition(x, localY);

    const auto ground = std::find_if(
        Palette::SOLID_GROUND.begin(), Palette::SOLID_GROUND.end(),
        [&color](const sf::Color& candidate) {
            return sameRgb(color, candidate);
        });
    if (ground != Palette::SOLID_GROUND.end()) {
        spawn.type = MapObjectType::SolidGround;
        spawn.variant = static_cast<int>(
            std::distance(Palette::SOLID_GROUND.begin(), ground));
    } else if (sameRgb(color, Palette::BRICK)) {
        spawn.type = MapObjectType::Brick;
    } else if (sameRgb(color, Palette::BREAKABLE_BRICK)) {
        spawn.type = MapObjectType::Brick;
        spawn.variant = 1;
    } else if (sameRgb(color, Palette::COIN_BLOCK)) {
        spawn.type = MapObjectType::CoinBlock;
    } else if (sameRgb(color, Palette::MUSHROOM_BLOCK)) {
        spawn.type = MapObjectType::MushroomBlock;
        spawn.variant = 1;
    } else if (sameRgb(color, Palette::GROW_MUSHROOM_BLOCK)) {
        spawn.type = MapObjectType::MushroomBlock;
        spawn.variant = 2;
    } else if (sameRgb(color, Palette::DOUBLE_HEAL_MUSHROOM_BLOCK)) {
        spawn.type = MapObjectType::MushroomBlock;
        spawn.variant = 3;
    } else if (color.r == 0 && color.b == 0 && color.g >= 250) {
        spawn.type = MapObjectType::Pipe;
        spawn.widthInTiles = 2;
        spawn.heightInTiles = std::max(1, 255 - static_cast<int>(color.g));
        spawn.variant = 255 - static_cast<int>(color.g);
    } else if (sameRgb(color, Palette::LAVA)) {
        spawn.type = MapObjectType::Lava;
    } else if (sameRgb(color, Palette::LAVA_BOTTOM)) {
        spawn.type = MapObjectType::LavaBottom;
    } else if (sameRgb(color, Palette::TRAMPOLINE)) {
        spawn.type = MapObjectType::Trampoline;
    } else if (sameRgb(color, Palette::CANNON_MID) ||
               sameRgb(color, Palette::CANNON_BOTTOM)) {
        spawn.type = MapObjectType::CannonBody;
        spawn.variant = sameRgb(color, Palette::CANNON_BOTTOM) ? 1 : 0;
    } else if (sameRgb(color, sf::Color::Black)) {
        spawn.type = MapObjectType::Cannon;
    } else if (color.r == 255 && color.g == 255 && color.b < 40) {
        spawn.type = MapObjectType::CloudPlatform;
        spawn.widthInTiles = 4;
        spawn.parameter = static_cast<float>(color.b) / 10.f;
    } else if (sameRgb(color, sf::Color::White)) {
        spawn.type = MapObjectType::WinFlag;
    } else if (sameRgb(color, Palette::COIN)) {
        spawn.type = MapObjectType::Coin;
    } else {
        addUnknownColor(color, x, localY, "object");
        return;
    }
    spawns.push_back(spawn);
}

void MapManager::decodeActorPixel(const sf::Color& color, unsigned int x,
                                  unsigned int localY) {
    if (isEmptyMapPixel(color)) return;

    MapSpawnInfo spawn;
    spawn.position = toWorldPosition(x, localY);
    if (sameRgb(color, Palette::PLAYER_1)) {
        spawn.type = MapObjectType::Player1Spawn;
    } else if (sameRgb(color, Palette::PLAYER_2)) {
        spawn.type = MapObjectType::Player2Spawn;
    } else if (sameRgb(color, Palette::GOOMBA)) {
        spawn.type = MapObjectType::Goomba;
    } else if (sameRgb(color, Palette::KOOPA)) {
        spawn.type = MapObjectType::Koopa;
    } else if (sameRgb(color, Palette::FLYING_KOOPA)) {
        spawn.type = MapObjectType::FlyingKoopa;
    } else if (sameRgb(color, Palette::HERISS)) {
        spawn.type = MapObjectType::Heriss;
    } else if (sameRgb(color, Palette::PETEY_PIRANHA)) {
        spawn.type = MapObjectType::PeteyPiranha;
    } else if (sameRgb(color, Palette::BOWSER)) {
        spawn.type = MapObjectType::Bowser;
    } else {
        addUnknownColor(color, x,
                        localY + MapFormat::LOGICAL_MAP_HEIGHT, "actor");
        return;
    }
    spawns.push_back(spawn);
}

void MapManager::addUnknownColor(const sf::Color& color, unsigned int x,
                                 unsigned int y, const char* layer) {
    std::ostringstream message;
    message << "Unknown " << layer << " color at (" << x << ", " << y
            << "): RGB(" << static_cast<int>(color.r) << ", "
            << static_cast<int>(color.g) << ", "
            << static_cast<int>(color.b) << ')';
    diagnostics.push_back({currentFile, {x, y}, color, message.str()});
    std::cerr << "MapManager: " << currentFile << ": " << message.str()
              << '\n';
}
