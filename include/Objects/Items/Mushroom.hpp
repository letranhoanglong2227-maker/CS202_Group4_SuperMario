#pragma once

#include "Objects/Items/Item.hpp"
class PlayerManager;

class Mushroom : public Item {
private:
    int type{2};
    sf::Vector2f restingPosition{};

public:
    Mushroom(const sf::Vector2f& pos, bool popped = false, int type = 2);
    virtual ~Mushroom() = default;

    ItemCollectionResult collect(PlayerManager& player, bool canGrow = true) override;

    void update(float dt) override;
};
