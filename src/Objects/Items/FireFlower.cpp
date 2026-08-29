#include "Objects/Items/FireFlower.hpp"
#include "Objects/Items/FireBuff.hpp"
#include "Core/Constants.hpp"
#include "Entities/Players/PlayerManager.hpp"
#include <memory>

FireFlower::FireFlower(const sf::Vector2f& pos, bool popped)
    : Item("FireFlower", 1000) {
    (void)popped;
    setPosition(pos);
    hitbox.setPosition(pos);
    hitbox.setSize({CELL_SIZE, CELL_SIZE});
    size = hitbox.getSize();
    
    entitySprite.setScale({2.0f, 2.0f});
    if (animationComponent) {
        animationComponent->addAnimation("idle", { 
            sf::IntRect({53, 36}, {16, 16}),
            sf::IntRect({70, 36}, {16, 16}),
            sf::IntRect({87, 36}, {16, 16}),
            sf::IntRect({104, 36}, {16, 16})
        });
    }
}

void FireFlower::onCollect(PlayerManager* player) {
    if (player && exist) {
        player->addBuff(std::make_unique<FireBuff>(-1.f)); // Infinite duration until hit
        exist = false;
    }
}

void FireFlower::update(float dt) {
    if (!exist) return;
    
    // Fireflower doesn't move, just animates
    if (animationComponent) {
        animationComponent->play("idle", dt);
    }
}
