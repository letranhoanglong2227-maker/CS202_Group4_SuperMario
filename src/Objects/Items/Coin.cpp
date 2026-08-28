#include "Objects/Items/Coin.hpp"
#include "Core/Constants.hpp"
#include "Entities/Players/PlayerManager.hpp"

Coin::Coin(const sf::Vector2f& pos, bool popped)
    : Item("Coin", 100), isPoppedFromBlock(popped), initialY(pos.y) {
    setPosition(pos);
    hitbox.setPosition(pos);
    hitbox.setSize({CELL_SIZE - 10.f, CELL_SIZE});
    size = hitbox.getSize();
}

void Coin::onCollect(PlayerManager* player) {
    (void)player;
    exist = false;
}

void Coin::reactToCollision() {
    exist = false;
}

void Coin::update(float dt) {
    if (!exist) return;

    if (isPoppedFromBlock) {
        floatTimer += dt;
        // Float upward and bounce down slightly when popped from Question block
        if (floatTimer < 0.25f) {
            position.y -= 120.f * dt;
        } else if (floatTimer < 0.5f) {
            position.y += 60.f * dt;
        } else {
            exist = false; // Disappear after pop animation
        }
        hitbox.setPosition(position);
        entitySprite.setPosition(position);
    } else {
        PowerUpObject::update(dt);
    }
}
