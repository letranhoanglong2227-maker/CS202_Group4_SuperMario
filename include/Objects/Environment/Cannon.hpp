#pragma once

#include "Objects/Blocks/Block.hpp"
#include <functional>
#include <memory>

class Cannon : public Block {
public:
    using SpawnCallback = std::function<void(std::unique_ptr<GameObject>)>;
    Cannon(sf::Vector2f position = {}, SpawnCallback spawn = {},
           float fireInterval = 2.5f, float direction = -1.f);
    void initSpritesSheet() override;
    void reactToCollision(int collidedSide) override;
    void update(float dt) override;
    void render(sf::RenderTarget* target) override;
private:
    SpawnCallback spawn;
    sf::RectangleShape shape;
    float fireInterval;
    float fireTimer{0.f};
    float direction;
};

