#pragma once

#include "Objects/Items/Item.hpp"
#include "Components/MovementComponent.hpp"
#include <memory>

class PlayerManager;

class FireFlower : public Item {
private:
    std::unique_ptr<MovementComponent> movementComponent;

public:
    FireFlower(const sf::Vector2f& pos, bool popped = false);
    
    virtual ~FireFlower() = default;
    ItemCollectionResult collect(PlayerManager& player) override;
    void update(float dt) override;
};
