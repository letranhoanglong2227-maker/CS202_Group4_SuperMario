#include "Levels/Managers/MapManager.hpp"
#include <iostream>

MapManager::MapManager(float gridSize) 
    : gridSize(gridSize <= 0.f ? 32.f : gridSize), spawnPointP1{0.f, 0.f}, spawnPointP2{0.f, 0.f} {}

bool MapManager::loadFromImage(const std::string& filename) {
    sf::Image image;
    if (!image.loadFromFile(filename)) {
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
