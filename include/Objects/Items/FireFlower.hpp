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
    
    void onCollect(PlayerManager* player);
    void update(float dt) override;
};
