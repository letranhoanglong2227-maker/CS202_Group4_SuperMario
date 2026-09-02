#include "Objects/Items/Mushroom.hpp"
#include "Core/Constants.hpp"
#include "Entities/Players/PlayerManager.hpp"
#include <algorithm>
#include <cmath>

Mushroom::Mushroom(const sf::Vector2f& pos, bool poppedFromBlock, int mushroomType)
    : Item("Mushroom", 1000), type(std::clamp(mushroomType, 1, 3)),
      restingPosition(pos) {
    popped = poppedFromBlock;
    setPosition(pos);
    hitbox.setSize({CELL_SIZE, CELL_SIZE});
    size = hitbox.getSize();

    entitySprite.setScale({2.0f, 2.0f});
    if (animationComponent) {
        animationComponent->addAnimation(
            "idle", {sf::IntRect({2 + 17 * (type - 1), 2}, {16, 16})});
    }
}

ItemCollectionResult Mushroom::collect(PlayerManager& player, bool canGrow) {
    if (!exist) return ItemCollectionResult{false, 0, 0, 0, RequestedPlayerForm::None};
    if (type == 2 && !canGrow)
        return ItemCollectionResult{false, 0, 0, 0, RequestedPlayerForm::None};

    exist = false;
    if (type == 2)
        return ItemCollectionResult{true, 0, 0, 0, RequestedPlayerForm::Big};

    // Group5's type 1 and 3 mushrooms heal one and two health respectively.
    player.setHealth(player.getHealth() + (type == 1 ? 1 : 2));
    return ItemCollectionResult{true, 0, 0, 0, RequestedPlayerForm::None};
}

void Mushroom::update(float dt) {
    if (!exist) return;

    if (popped) {
        popTimer = std::min(popTimer + std::max(0.f, dt), popDuration);
        const float progress = popDuration > 0.f ? popTimer / popDuration : 1.f;
        position = restingPosition;
        position.y -= std::sin(progress * 3.14159265f) * 24.f;
        if (popTimer >= popDuration) popped = false;
        hitbox.setPosition(position);
        entitySprite.setPosition(position);
    }

    if (animationComponent) {
        animationComponent->play("idle", dt);
    }
}
