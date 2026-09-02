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
            sf::IntRect({2, 19}, {16, 16}),
            sf::IntRect({19, 19}, {16, 16}),
            sf::IntRect({36, 19}, {16, 16}),
            sf::IntRect({53, 19}, {16, 16})
        });
    }
}

ItemCollectionResult FireFlower::collect(PlayerManager& player, bool canGrow) {
    if (!exist) return ItemCollectionResult{false, 0, 0, 0, RequestedPlayerForm::None};
    if (!player.isBig() && !canGrow) return ItemCollectionResult{false, 0, 0, 0, RequestedPlayerForm::None};
    
    exist = false;
    return ItemCollectionResult{true, 1000, 0, 0, RequestedPlayerForm::Fire};
}

void FireFlower::update(float dt) {
    if (!exist) return;
    
    // Fireflower doesn't move, just animates
    if (animationComponent) {
        animationComponent->play("idle", dt);
    }
}
