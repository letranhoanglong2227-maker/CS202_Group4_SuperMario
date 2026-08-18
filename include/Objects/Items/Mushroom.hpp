#pragma once

#include "Objects/Items/Item.hpp"
#include "Components/MovementComponent.hpp"
#include <memory>

class PlayerManager;

class Mushroom : public Item {
private:
    float moveSpeed{ 60.f };
    float dirX{ 1.f };
    std::unique_ptr<MovementComponent> movementComponent;

public:
    Mushroom(const sf::Vector2f& pos = { 0.f, 0.f }, bool popped = false);
    ~Mushroom() override = default;

    void onCollect(PlayerManager* player);
    void reverseDirection();
    void reactToBlockCollision(Block* block) override;

    void update(float dt) override;
};
