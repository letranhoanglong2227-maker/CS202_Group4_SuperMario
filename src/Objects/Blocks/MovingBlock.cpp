#include "Objects/Blocks/MovingBlock.hpp"
#include "Levels/Managers/MapManager.hpp"
#include <algorithm>
#include <cmath>
#include <filesystem>
#include <string>

namespace {
sf::IntRect textureRectFor(std::string_view textureName) {
    if (textureName == "basic_ground_mid") {
        return {{18, 1}, {16, 16}};
    }
    if (textureName == "basic_leafground_mid") {
        return {{120, 120}, {16, 16}};
    }
    if (textureName == "snow_ground_mid") {
        return {{154, 1}, {16, 16}};
    }
    return {};
}

std::string tilesetPath() {
    std::filesystem::path candidate = "assets/textures/Tileset.png";
    for (int depth = 0; depth < 3; ++depth) {
        if (std::filesystem::exists(candidate)) return candidate.string();
        candidate = std::filesystem::path("..") / candidate;
    }
    return "assets/textures/Tileset.png";
}
}

MovingBlock::MovingBlock(sf::Vector2f pos, int widthInTiles,
                         sf::Vector2f distance, float moveSpeed)
    : origin(pos), travel(distance), speed(std::max(0.f, moveSpeed)) {
    setPosition(pos);
    setSize({static_cast<float>(std::max(1, widthInTiles)) * MapFormat::TILE_SIZE,
             MapFormat::TILE_SIZE});
    shape.setPosition(pos);
    shape.setSize(getSize());
    shape.setFillColor(sf::Color(238, 238, 238));
}

MovingBlock::MovingBlock(sf::Vector2f pos, int widthInTiles,
                         float distance, float moveSpeed,
                         std::string_view textureName)
    : MovingBlock(pos, widthInTiles, sf::Vector2f{}, moveSpeed) {
    squareDistance = std::max(0.f, distance);
    path = Path::SquareLoop;

    const sf::IntRect textureRect = textureRectFor(textureName);
    if (textureRect.size.x > 0 && entityTexture.loadFromFile(tilesetPath())) {
        entitySprite.setTexture(entityTexture, true);
        entitySprite.setTextureRect(textureRect);
        entitySprite.setScale(
            {getSize().x / static_cast<float>(textureRect.size.x),
             getSize().y / static_cast<float>(textureRect.size.y)});
        entitySprite.setPosition(pos);
        textured = true;
    }
}

void MovingBlock::update(float dt) {
    const sf::Vector2f previous = getPosition();
    if (path == Path::SquareLoop && squareDistance > 0.f && dt > 0.f) {
        const float perimeter = 4.f * squareDistance;
        squareProgress = std::fmod(squareProgress + speed * dt, perimeter);

        sf::Vector2f offset{};
        if (squareProgress <= squareDistance) {
            offset.x = squareProgress;
        } else if (squareProgress <= 2.f * squareDistance) {
            offset.x = squareDistance;
            offset.y = squareProgress - squareDistance;
        } else if (squareProgress <= 3.f * squareDistance) {
            offset.x = 3.f * squareDistance - squareProgress;
            offset.y = squareDistance;
        } else {
            offset.y = perimeter - squareProgress;
        }
        setPosition(origin + offset);
    } else {
        const float distance = std::sqrt(travel.x * travel.x + travel.y * travel.y);
        if (distance > 0.f && dt > 0.f) {
            float phase = direction > 0.f ? progress : 2.f - progress;
            phase = std::fmod(phase + speed * dt / distance, 2.f);
            if (phase <= 1.f) {
                progress = phase;
                direction = phase < 1.f ? 1.f : -1.f;
            } else {
                progress = 2.f - phase;
                direction = -1.f;
            }
            setPosition(origin + travel * progress);
        }
    }
    frameDelta = getPosition() - previous;
    shape.setPosition(getPosition());
}

void MovingBlock::render(sf::RenderTarget* target) {
    if (!target) return;
    if (textured) target->draw(entitySprite);
    else target->draw(shape);
}

sf::Vector2f MovingBlock::getFrameDelta() const noexcept { return frameDelta; }

