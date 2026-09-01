#pragma once

#include "Objects/Blocks/Block.hpp"

class CloudBlock final : public Block {
public:
    CloudBlock(sf::Vector2f position = {}, int widthInTiles = 4,
               float initialTime = 0.f);
    void initSpritesSheet() override;
    void reactToCollision(int collidedSide) override;
    void update(float dt) override;
    void render(sf::RenderTarget* target) override;
    bool canBeRemoved() const override { return false; }
private:
    float timer{};
    static constexpr float visibleDuration = 4.f;
    static constexpr float hiddenDuration = 2.5f;
};
