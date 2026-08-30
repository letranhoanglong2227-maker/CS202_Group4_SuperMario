#pragma once

#include "Core/Constants.hpp"
#include "Objects/Blocks/SolidBlock.hpp"
#include <string_view>

class MovingBlock : public SolidBlock {
public:
    MovingBlock(sf::Vector2f position = {}, int widthInTiles = 2,
                sf::Vector2f travel = {3.f * CELL_SIZE, 0.f},
                float speed = 60.f);
    MovingBlock(sf::Vector2f position, int widthInTiles,
                float squareDistance, float speed,
                std::string_view textureName = {});
    void update(float dt) override;
    void render(sf::RenderTarget* target) override;
    sf::Vector2f getFrameDelta() const noexcept;
private:
    enum class Path { LinearPingPong, SquareLoop };

    sf::Vector2f origin;
    sf::Vector2f travel;
    sf::Vector2f frameDelta{};
    sf::RectangleShape shape;
    float speed;
    float progress{0.f};
    float direction{1.f};
    float squareDistance{0.f};
    float squareProgress{0.f};
    Path path{Path::LinearPingPong};
    bool textured{false};
};

