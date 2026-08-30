#pragma once

#include "Objects/Items/Item.hpp"

class PlayerManager;

class Coin : public Item {
private:
    float floatTimer{ 0.f };
    bool isPoppedFromBlock{ false };
    float initialY{ 0.f };

public:
    Coin(const sf::Vector2f& pos, bool popped = false);
    virtual ~Coin() = default;

    ItemCollectionResult collect(PlayerManager& player) override;
    void reactToCollision() override;

    void update(float dt) override;
};
