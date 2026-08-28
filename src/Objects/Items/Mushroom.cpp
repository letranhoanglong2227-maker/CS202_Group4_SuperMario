#include "Objects/Items/Mushroom.hpp"
#include "Core/Constants.hpp"
#include "Entities/Players/PlayerManager.hpp"

Mushroom::Mushroom(const sf::Vector2f& pos, bool popped)
    : Item("Mushroom", 1000) {
    (void)popped;
    setPosition(pos);
    hitbox.setPosition(pos);
    hitbox.setSize({CELL_SIZE, CELL_SIZE});
    size = hitbox.getSize();
    movementComponent = std::make_unique<MovementComponent>(60.f, 500.f, 0.f);
}

void Mushroom::onCollect(PlayerManager* player) {
    if (player && exist) {
        player->setBig(true);
        exist = false;
    }
}

void Mushroom::reverseDirection() {
    dirX = -dirX;
}

void Mushroom::reactToBlockCollision(Block* block) {
    (void)block;
    reverseDirection();
}

void Mushroom::update(float dt) {
    if (!exist) return;

    if (movementComponent) {
        movementComponent->move(dirX, 0.f, dt);
        movementComponent->update(dt);
        position += movementComponent->getVelocity() * dt;
        hitbox.setPosition(position);
        entitySprite.setPosition(position);
    }
}
